// 驾驶员眼部立体匹配与三维重构.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv.hpp"

using namespace std;
using namespace cv;

Mat cameraMatrixL,cameraDistcoeffL,cameraMatrixR,cameraDistcoeffR;
Mat R, T,Q, R1, P1, R2, P2;
Rect roiL,roiR;
Mat mapLx,mapLy,mapRx,mapRy;

//Point3f myReprojectImageTo3D(Point3f data, Mat Q){
//	float X = data[0]-Q[0][3];
//}

int main()
{
	//加载眼部分类器
	CascadeClassifier eye_cf;
	if(!eye_cf.load("..\\eye_classifier\\haarcascade_eye.xml")){
		cout<<"load eye classifier error!";
		return 0;
	}

	//加载双目标定参数
	FileStorage fs("..\\calib_parame\\doubleCameraCalib.xml", FileStorage::READ);
	if(!fs.isOpened()){
		cout<<"load file -- doubleCameraCalib.xml -- error! ";
		fs.release();
		return 0;
	}else{
		fs["cameraMatrixL"] >> cameraMatrixL;
		fs["cameraDistcoeffL"] >> cameraDistcoeffL;
		fs["cameraMatrixR"] >> cameraMatrixR;
		fs["cameraDistcoeffR"] >> cameraDistcoeffR;
		fs["R"] >> R;
		fs["T"] >> T;
		fs.release();
	}
	
	//双目校正
	stereoRectify( cameraMatrixL, cameraDistcoeffL, cameraMatrixR, cameraDistcoeffR, 
		Size(640,480), R, T, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, -1, Size(640,480), &roiL, &roiR );
	cout<<Q<<endl;
	//获得映射矩阵
	initUndistortRectifyMap(cameraMatrixL, cameraDistcoeffL, R1, P1, Size(640,480), CV_16SC2, mapLx, mapLy);
	initUndistortRectifyMap(cameraMatrixR, cameraDistcoeffR, R2, P2, Size(640,480), CV_16SC2, mapRx, mapRy);

	VideoCapture capL(0),capR(1);
	capL.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capL.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	capR.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	capR.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	while(1){
		Mat frameL,frameR,grayImgL,grayImgR;
		capL >> frameL;
		capR >> frameR;
		cvtColor(frameL, grayImgL, CV_BGR2GRAY);
		cvtColor(frameR, grayImgR, CV_BGR2GRAY);
		remap(grayImgL, grayImgL, mapLx, mapLy, INTER_LINEAR);
		remap(grayImgR, grayImgR, mapRx, mapRy, INTER_LINEAR);

		vector<Rect> eye;
		eye_cf.detectMultiScale(grayImgL,eye,1.1,3,0,Size(50,50),Size(100,100));
		if(eye.size() != 2){
			continue;
		}
		imwrite("..\\image_eye\\left01.jpg", frameL);
		imwrite("..\\image_eye\\right01.jpg", frameR);
		for(vector<Rect>::const_iterator iter=eye.begin();iter!=eye.end();iter++){
//			rectangle(grayImgL,*iter,Scalar(0),2,8);
//			rectangle(grayImgR,Rect(0,iter->y,640,iter->height),Scalar(0),2,8);
			Mat roiL = grayImgL(*iter);
			Mat match_img = grayImgR(Rect(0,iter->y,640,iter->height));

			Mat cal_value;
			matchTemplate(match_img,roiL,cal_value,CV_TM_CCORR);
			double minVal, maxVal;
			Point minLoc, maxLoc;
			minMaxLoc(cal_value, &minVal, &maxVal, &minLoc, &maxLoc);
			circle(grayImgR,Point(minLoc.x,minLoc.y+iter->y),100,0);
			//Point3d data(minLoc.x,iter->y+minLoc.y,fabs(minLoc.x-iter->x-iter->width/2));
			//Point3f XYZ= myReprojectImageTo3D(data, Q);
		}
		 
		imshow("左相机视频", grayImgL);
		imshow("右相机视频", grayImgR);
		char oper = waitKey(0);
		if(oper == 27)
			break;
	}
	return 0;
}

