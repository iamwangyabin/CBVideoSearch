#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//VideoCapture::VideoCapture(const string& filename); 

int keyframes(const char * filename) {
	// 打开一个文件 
	CvCapture* capture;
	capture = cvCreateFileCapture(filename);
	// 获取帧数 
	long numFrames = (long)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	// 获取帧率 
	int fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	// 承载每一帧 
	IplImage *frame;
	int i = 0;
	while (1) {
		frame = cvQueryFrame(capture);
		if (!frame)
			break;
		if (i % 25 == 0) {
			char* image_name;
			sprintf(image_name, "%s%d%s", "..\\tutu\\image", ++i, ".jpg");
			cvSaveImage(image_name, frame);
		}
		else {
			continue;
		}
	}
	cvReleaseCapture(&capture);
}