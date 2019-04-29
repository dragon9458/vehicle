// ˫Ŀ����궨��ͼ��ɼ�.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "opencv.hpp"

using namespace std;
using namespace cv;

Mat frameL,frameR;

int main()
{
	//��˫Ŀ�����������ͼ��ֱ���640*480
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
		imshow("�������Ƶ",frameL);
		imshow("�������Ƶ",frameR);
		imwrite(filenameL,frameL);
		imwrite(filenameR,frameR);
		currNum++;
		char oper = waitKey(2000);
		if(oper == 27)
			break;
	}

	return 0;
}

