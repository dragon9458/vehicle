// �궨��.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "opencv.hpp"

using namespace cv;
using namespace std;


int main()
{
	Mat srcImg(600,420,CV_8UC3,Scalar(255,255,255));
	for(int i = 0; i < srcImg.rows; ++i){
		if(i/60 %2 == 0){
			for(int j =0; j< srcImg.cols;++j){
				if(j/60%2 ==0){
					srcImg.at<cv::Vec3b>(i,j)[0]=0;
					srcImg.at<cv::Vec3b>(i,j)[1]=0;
					srcImg.at<cv::Vec3b>(i,j)[2]=0;
				}
			}
		}
		else{
			for(int j =0; j< srcImg.cols;++j){
				if(j/60%2 ==1){
					srcImg.at<cv::Vec3b>(i,j)[0]=0;
					srcImg.at<cv::Vec3b>(i,j)[1]=0;
					srcImg.at<cv::Vec3b>(i,j)[2]=0;
				}

			}
		
		}
	
	}
	imwrite("..\\aa.jpg", srcImg);
	imshow("aa",srcImg);
	waitKey(0);
	return 0;
}

