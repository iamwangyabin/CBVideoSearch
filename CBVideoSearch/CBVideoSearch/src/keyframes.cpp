#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

//VideoCapture::VideoCapture(const string& filename); 

int main(){
	// 打开一个文件 
	VideoCapture capture(const string& filename);
	capture.open(const string& filename);
	if ( !capture.isOpened( ) )
        cout << "fail toopen!" << endl;
    // 获取帧数 
    long totalFrameNumber = capture.get( CV_CAP_PROP_FRAME_COUNT );
    // 设置开始帧 
    long frameToStart = 1;
    capture.set( CV_CAP_PROP_POS_FRAMES, frameToStart );
    // 获取帧率 
    double rate = capture.get( CV_CAP_PROP_FPS );
    // 承载每一帧 
    Mat frame; 
    int i=0;
    while(1){
    	frame = cvQueryFrame(capture);
    	if(frame == NULL)
    		break;
    	if(i%25 == 0){
    		sprintf(image_name, "%s%d%s", "..\\tutu\\image", ++i, ".jpg");
    		cvSaveImage(image_name, frame);
		}
		else{
			continue;
		}
	} 
    
	capture.release();
	//bool VideoCapture::open(const string& filename)
}



