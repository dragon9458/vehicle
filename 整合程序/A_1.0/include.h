/*********************************ͷ�ļ�***********************************/
#include "opencv.hpp"

#include <fstream>
#include <sstream>

#include <process.h>  
#include <windows.h>  

#include "CameraDS.h"


/*********************************�����ռ�**********************************/
using namespace cv;
using namespace std;

#define		A_IMAGE		"A��ͼ��"
#define     M_IMAGE     "ä��ͼ��"

/*********************************�ֲ�����**********************************/
Mat	imageProcess();
void changeMat ();

/********************************ȫ�ֱ���**********************************/
Mat frame,frame1;				
Mat dstImage,dstImage1;			
Mat imageCut;
Mat imageRot, imageRot1;
Mat	rotMat,rotMat1;
Mat rotImage;
Mat matRot;

FileStorage fs;

Point2f srcPoint[4];					//Դͼ���4����������
Point2f warpPoint[4];					//Ŀ��ͼ���4����������
Point2f srcPoint1[4];					//Դͼ���4����������
Point2f warpPoint1[4];					//Ŀ��ͼ���4����������

int p1x = 1000 , p1y = -280 ,
	p2x = 1033 , p2y = 1090 ,
	p3x = 280 ,	p3y = -280 , 
	p4x = 202 , p4y = 928 ,

	base = 36 ,
	flag = 1 ,
	release = 1;
float angle =0.0, scale =1.0;
Point center =Point(640,360);