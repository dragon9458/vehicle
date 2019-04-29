/*********************************头文件***********************************/
#include "opencv.hpp"

#include <fstream>
#include <sstream>

#include <process.h>  
#include <windows.h>  

#include "CameraDS.h"


/*********************************命名空间**********************************/
using namespace cv;
using namespace std;

#define		A_IMAGE		"A柱图像"
#define     M_IMAGE     "盲区图像"

/*********************************局部函数**********************************/
Mat	imageProcess();
void changeMat ();

/********************************全局变量**********************************/
Mat frame,frame1;				
Mat dstImage,dstImage1;			
Mat imageCut;
Mat imageRot, imageRot1;
Mat	rotMat,rotMat1;
Mat rotImage;
Mat matRot;

FileStorage fs;

Point2f srcPoint[4];					//源图像的4个顶点坐标
Point2f warpPoint[4];					//目标图像的4个顶点坐标
Point2f srcPoint1[4];					//源图像的4个顶点坐标
Point2f warpPoint1[4];					//目标图像的4个顶点坐标

int p1x = 1000 , p1y = -280 ,
	p2x = 1033 , p2y = 1090 ,
	p3x = 280 ,	p3y = -280 , 
	p4x = 202 , p4y = 928 ,

	base = 36 ,
	flag = 1 ,
	release = 1;
float angle =0.0, scale =1.0;
Point center =Point(640,360);