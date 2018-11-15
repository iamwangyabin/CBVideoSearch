#include "stdint.h" //重点是这一句
//    -fexec-charset=GBK
//    -finput-charset=UTF-8

#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <utility>

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
