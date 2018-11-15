#ifndef __BOW_BUILDER__
#define __BOW_BUILDER__

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <utility>
#include "cppsugar/cppsugar"
#ifdef _OPENMP
#include <omp.h>
#endif

using namespace std;
using namespace util;

struct BoW
{
    int id;
    vector<float> bow;
    BoW(): id(-1) {};
    BoW(int _id, const std::vector<float> &_bow): id(_id), bow(_bow) {};

    void Serialize(ofstream &ofs) const {
        ofs.write((char *)&id, sizeof(int));
        int size = bow.size();
        ofs.write((char *)&size, sizeof(int));
        ofs.write((char *)&bow[0], sizeof(float) * size);
    }

    static BoW Deserialize(ifstream &ifs) {
        BoW bow;
        ifs.read((char *)&bow.id, sizeof(int));
        int size = 0;
        ifs.read((char *)&size, sizeof(int));
        bow.bow.resize(size);
        ifs.read((char *)&bow.bow[0], sizeof(float) * size);
        return bow;
    }
};

struct BoWCollection
{
    vector<BoW> bows;
    BoWCollection() {}
    BoWCollection(int _size): bows(_size) {}

    void Serialize(ofstream &ofs) const {
        int size = bows.size();
        ofs.write((char *)&size, sizeof(int));
        for (const auto &bow : bows) {
            bow.Serialize(ofs);
        }
    }

    static BoWCollection Deserialize(ifstream &ifs) {
        BoWCollection bows;
        int size = 0;
        ifs.read((char *)&size, sizeof(int));
        for (int i = 0; i < size; i++) {
            auto bow = BoW::Deserialize(ifs);
            bows.bows.push_back(bow);
        }
        return bows;
    }
};

class BoWBuilder
{
public:
    // some default settings
    const int DICT_SIZE = 100000;    // vitual words
    //const int DICT_SIZE = 10000;
    const int FEATURE_DIMENSION = 128;    // for SIFT
    const int KMEANS_MAX_ITERATION = 75;

    CompositeIndexParams QuantizationIndex;

    BoWBuilder(void) {};
    ~BoWBuilder(void) {};

    Mat ExtractSIFTFeature(const string &imgfn) const {
        // 这个就是提取一幅图片中所有的特征，返回这个矩阵
    }

    // 这个是把所有图的特征都集合起来
    // Extract sparse SIFT feature from the given images, the returned value is a vector of feature.
    // Each vector contains the descriptors of each image.
    vector<Mat> ExtractSIFTFeatures(const vector<string> &imgfns) const {
        vector<Mat> features(imgfns.size());
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for (int i = 0; i < imgfns.size(); i++) {
            features[i] = ExtractSIFTFeature(imgfns[i]);
        }
        return features;
    }

    // build a codebook from a file containing the file names of the images, sparse SIFT is used.
    // k is the vocabulary size. The features are returned as features.
    // hard approximate quantization is also done using the intermediate result of KMeans
    Mat BuildCodebookAndQuantize(const vector<string> &imgfns, vector<Mat> &features, BoWCollection &bows) const
    {
        auto c = clock();

        int k = BoWBuilder::DICT_SIZE;
        // for our current scale, don't need to subsample the features
        features = ExtractSIFTFeatures(imgfns);
        cerr << "Feature extraction complete." << endl;
        int totalNum = 0;
        for (int i = 0; i < features.size(); i++) {
            totalNum += features[i].rows;
        }
        // int totalNum = Sum<Mat, int>(features, [](const Mat &v) { return v.rows; });
        cerr << totalNum << " features extracted. " << k << " visual words to cluster, " << (float)(totalNum * 100 / k) / 100
        << " features per word in average." << endl;
        // first randomly generate the cluster centers
        cerr << "Initializing centers...";
        vector<int> centerIds(totalNum);
        for (int i = 0; i < totalNum; i++) centerIds[i] = i;
        random_shuffle(centerIds.begin(), centerIds.end());
        set<int> centerIdSet;
        for (int i = 0; i < k; i++) centerIdSet.insert(centerIds[i]);
        centerIds.clear();
        cerr << "done.\nConstruct data structure..." << endl;

        // copy the selected features to centersMat
        Mat centersMat(k, BoWBuilder::FEATURE_DIMENSION, DataType<float>::type);
        int currentIdx = 0, centersIdx = 0;
        for (auto it = features.begin(); it != features.end(); it++)
        {
            for (int i = 0; i < it->rows; i++)
            {
                if (centerIdSet.find(currentIdx) != centerIdSet.end())
                {
                    memcpy((char *)(centersMat.datastart + centersIdx * centersMat.step), it->datastart + i * it->step, sizeof(float) * FEATURE_DIMENSION);
                    centersIdx++;
                }
                currentIdx++;
            }
        }

        // actual iteration
        cerr << "Actual iteration...";
        vector<int> labels(totalNum);
        Mat centersSum(k, FEATURE_DIMENSION, DataType<float>::type);
        // for more effient parallelization, use a two-stage indexing system
        vector<pair<int, int>> idx;
        idx.reserve(totalNum);
        for(int i = 0; i < features.size(); i++)
        {
            for (int j = 0; j < features[i].rows; j++) {
                idx.push_back(pair<int, int>(i, j));
            }
        }

        for (int iter = 0; iter < KMEANS_MAX_ITERATION; iter++)
        {
            // build index
            Index index(centersMat, KDTreeIndexParams());
            // assign to the cluster centers
            vector<int> labelCount(k);
            memset(centersSum.datastart, 0, centersSum.dataend - centersSum.datastart);
#ifdef _OPENMP
#pragma omp parallel for
#endif
            for (int id = 0; id < idx.size(); id++) {
                int i = idx[id].first;
                int j = idx[id].second;
                Mat nn(1, 1, DataType<int>::type);
                Mat dist(1, 1, DataType<float>::type);
                index.knnSearch(features[i].row(j), nn, dist, 1, SearchParams());
                int label = nn.at<int>(0, 0);
                centersSum.row(label) += features[i].row(j);
                labelCount[label]++;
            }
            for (int i = 0; i < k; i++)
                if (labelCount[i])
                    centersSum.row(i) /= labelCount[i];
            //actually updating the cluster centers
            centersMat = centersSum.clone();
            cerr << '.';
        }
#ifdef __linux__
        cerr << "done. " << (clock() - c) / 10000000 << "sec." << endl;
#else
        cerr << "done. " << (clock() - c) / 1000 << "sec." << endl;
#endif

        // do a last round for quantization
        cerr << "Quantization... " << endl;
        Index index(centersMat, KDTreeIndexParams());
        bows = BoWCollection();
        bows.bows.resize(features.size());
#ifdef _OPENMP
#pragma omp parallel for
#endif
        for(int i = 0; i < features.size(); i++)
        {
            if (!features[i].rows) {
                bows.bows[i] = BoW(i, vector<float>(k));
                continue;
            }
            Mat nn(features[i].rows, 1, DataType<int>::type);
            Mat dist(features[i].rows, 1, DataType<float>::type);
            index.knnSearch(features[i], nn, dist, 1, SearchParams());
            vector<float> bow(k);
            for (int j = 0; j < features[i].rows; j++) {
                bow[(int)nn.at<int>(j, 0)]++;
            }
            // L1 normalization
            float bowSum = Sum(bow);
            for (auto &b : bow) { b /= bowSum; }
            bows.bows[i] = BoW(i, bow);

            // L2 normalization
            //float accum = sqrt(std::inner_product(bows.bows[i].bow.begin(), bows.bows[i].bow.end(), bows.bows[i].bow.begin(), 0.0));
            //for (auto &b : bows.bows[i].bow) { b /= accum; }

        }

        return centersMat;
    }

    Mat ReadCodebook() const {
        vector<float> buff;
        ifstream ifs("dict.dat", ios::binary);
        if (!ifs) { throw runtime_error("Cannot open file."); };
        int w = 0, h = 0;
        ifs.read((char *)&h, sizeof(int));
        ifs.read((char *)&w, sizeof(int));
        buff.resize(w * h);
        ifs.read((char *)&buff[0], sizeof(float) * buff.size());
        ifs.close();

        // row major
        return Mat(buff, true).reshape(1, h);
    }

    void WriteCodebook(const Mat &dict) const {
        ofstream ofs("dict.dat", ios::binary);
        if (!ofs) { throw runtime_error("Cannot open file."); }
        ofs.write((char *)&dict.rows, sizeof(int));
        ofs.write((char *)&dict.cols, sizeof(int));
        ofs.write((char *)dict.data, dict.dataend - dict.data);
        ofs.close();
    }

    // extract features from the given image and the roi, and then return the quantization/pooling result.
    vector<float> Quantize(const Mat &dict, string imgfn) const {
        auto feature = ExtractSIFTFeature(imgfn);
        Index index(dict, KDTreeIndexParams());
        Mat nn(feature.rows, 1, DataType<int>::type);
        Mat dist(feature.rows, 1, DataType<float>::type);
        index.knnSearch(feature, nn, dist, 1, SearchParams());
        vector<float> bow(dict.rows);
        for (int j = 0; j < feature.rows; j++) {
            bow[nn.at<int>(j, 0)]++;
        }
        // L1 normalization
        float bowSum = Sum(bow);
        for (auto &b : bow) { b /= bowSum; }

        // l2-norm
        //float accum = sqrt(std::inner_product(bow.begin(), bow.end(), bow.begin(), 0.0));
        //for (auto &b : bow) { b /= accum; }

        return bow;
    }
};

#endif