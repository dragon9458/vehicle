// test1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "opencv.hpp"

using namespace cv;
using namespace std;


int main()
{
	Mat srcImg(560,480,CV_8SC1,Scalar(255));
	imshow("aa",srcImg);
	waitKey(0);
	return 0;
}

