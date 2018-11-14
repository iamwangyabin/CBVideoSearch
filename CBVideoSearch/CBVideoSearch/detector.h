#pragma once
#ifndef DETECTOR_H
#define DETECTOR_H

#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <opencv2/opencv.hpp>

extern "C" {
#include "vlfeat/vl/sift.h"
}
namespace CBVS {
	class SiftDetector {

	public:
		explicit SiftDetector(double in_peak_threshold = 5, double in_edge_threshold = 5);
		void detect_and_compute(const cv::Mat &img, std::vector<VlSiftKeypoint> &kpts, std::vector<std::vector<float>> &descriptors, bool root_sift = false);
		static void save_sift(const std::string &filename, const std::vector<std::vector<float>> &descriptors);
		static void load_sift(const std::string &filename, std::vector<std::vector<float>> &desceriptors);

	private:
		/*
		用VLfeat来提取sift
		parameters:
		vl_sfit, VlSiftFilt*
		data , image pixel data ,to be convert to float
		kpts, keypoint list
		descriptors, descriptor. Need to free the memory after using.
		*/
		void vl_sift_extract(VlSiftFilt *vl_sift, vl_sift_pix* data,
			std::vector<VlSiftKeypoint> &kpts, std::vector<std::vector<float>> &descriptors);

		/*
		*  转换成 root-sift
		* */
		void convert_sift_to_root_sift(std::vector<std::vector<float>> &descriptors);
	private:
		double peak_threshold;
		double edge_threshold;
	};
}

#endif