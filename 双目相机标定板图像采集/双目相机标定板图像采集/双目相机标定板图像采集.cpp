// 双目相机标定板图像采集.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv.hpp"

using namespace std;
using namespace cv;

Mat frameL,frameR;

int main()
{
	//打开双目摄像机，设置图像分辨率640*480
	VideoCapture capL(0);
	capL.set(CV_CAP_PROP_FRAME_WIDTH,640);
	capL.set(CV_CAP_PROP_FRAME_HEIGHT,480);
	VideoCapture capR(1);
	capR.set(CV_CAP_PROP_FRAME_WIDTH,640);
	capR.set(CV_CAP_PROP_FRAME_HEIGHT,480);

	int currNum = 0;

	while(1){
		char filenameL[100],filenameR[100];
		sprintf_s(filenameL, "..\\calib_image\\left%02d.jpg", currNum +1 );
		sprintf_s(filenameR, "..\\calib_image\\right%02d.jpg", currNum + 1);
		capL>>frameL;
		capR>>frameR;
		imshow("左相机视频",frameL);
		imshow("右相机视频",frameR);
		imwrite(filenameL,frameL);
		imwrite(filenameR,frameR);
		currNum++;
		char oper = waitKey(2000);
		if(oper == 27)
			break;
	}

	return 0;
}

