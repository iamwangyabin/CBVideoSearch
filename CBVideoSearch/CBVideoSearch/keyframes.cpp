#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//VideoCapture::VideoCapture(const string& filename); 

int keyframes(const char * filename) {
	// ��һ���ļ� 
	CvCapture* capture;
	capture = cvCreateFileCapture(filename);
	// ��ȡ֡�� 
	long numFrames = (long)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	// ��ȡ֡�� 
	int fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	// ����ÿһ֡ 
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