#include "detector.h"
#include "stdafx.h"

using namespace std;
using namespace cv;
using namespace CBVS;
CBVS::SiftDetector::SiftDetector(double in_peak_threshold, double in_edge_threshold) : peak_threshold(in_peak_threshold), edge_threshold(in_edge_threshold) {}

void CBVS::SiftDetector::detect_and_compute(const cv::Mat &img, std::vector<VlSiftKeypoint> &kpts,
	std::vector<std::vector<float>> &descriptors, bool root_sift) {
	assert(img.channels() == 1 || img.channels() == 3);
	Mat gray;
	if (img.channels() == 3) {
		cvtColor(img, gray, CV_RGB2GRAY);
	}
	else {
		gray = img;
	}
	VlSiftFilt *vl_sift = vl_sift_new(img.cols, img.rows, -1, 3, 0);
	vl_sift_set_edge_thresh(vl_sift, edge_threshold);
	vl_sift_set_peak_thresh(vl_sift, peak_threshold);

	Mat float_img;
	gray.convertTo(float_img, CV_32F);

	auto data = (vl_sift_pix*)float_img.data;
	vl_sift_extract(vl_sift, data, kpts, descriptors);
	if (root_sift) {
		convert_sift_to_root_sift(descriptors);
	}

	vl_sift_delete(vl_sift);
}

void CBVS::SiftDetector::vl_sift_extract(VlSiftFilt *vl_sift, vl_sift_pix *data, std::vector<VlSiftKeypoint> &kpts,
	std::vector<std::vector<float>> &descriptors) {
	if (vl_sift_process_first_octave(vl_sift, data) != VL_ERR_EOF) {
		while (true)
		{
			vl_sift_detect(vl_sift);

			VlSiftKeypoint* pKpts = vl_sift->keys;
			for (int i = 0; i < vl_sift->nkeys; i++) {
				double angles[4];
				// ����������ķ������������͸���������ĸ�
				int angleCount = vl_sift_calc_keypoint_orientations(vl_sift, angles, pKpts);
				// ���ڷ������һ���������㣬ÿ������ֱ��������������
				// ���ҽ������㸴�ƶ��
				for (int i = 0; i < angleCount; i++)
				{
					vector<float> des(128);
					vl_sift_calc_keypoint_descriptor(vl_sift, des.data(), pKpts, angles[0]);
					descriptors.push_back(des);
					kpts.push_back(*pKpts);
				}
				pKpts++;
			}
			if (vl_sift_process_next_octave(vl_sift) == VL_ERR_EOF) {
				break;
			}
		}
	}
}

void CBVS::SiftDetector::convert_sift_to_root_sift(std::vector<std::vector<float>> &descriptors) {
	/*
	Trans sift to rootSift
	1. L1 normalize each descriptor(128-dims vector)
	2. sqrt-root each element
	3. [option] L2-normalize descriptor vector
	*/
	for (vector<float> &vec : descriptors) {
		float sum = 0;
		auto ptr = vec.data();
		for (int i = 0; i < 128; i++)
			sum += *(ptr + i);
		for (int i = 0; i < 128; i++) {
			*(ptr + i) /= sum;
			*(ptr + i) = sqrt(*(ptr + i));
		}
	}
}