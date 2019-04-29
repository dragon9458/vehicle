// 左右相机标定.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "opencv.hpp"

using namespace std;
using namespace cv;

const int imageWidth = 640;                             //摄像头的分辨率  
const int imageHeight = 480;  
const int boardWidth = 9;                               //横向的角点数目  
const int boardHeight = 6;                              //纵向的角点数据  
const int boardCorner = boardWidth * boardHeight;       //总的角点数据  
const int frameNumber = 28;                             //相机标定时需要采用的图像帧数  
const int squareSize = 41;                              //标定板黑白格子的大小 单位mm  
const Size boardSize = Size(boardWidth, boardHeight);   //  
Size imageSize = Size(imageWidth, imageHeight);  
  
Mat R, T, E, F;                                         //R 旋转矢量 T平移矢量 E本征矩阵 F基础矩阵    
vector<vector<Point2f>> imagePointL;                    //左边摄像机所有照片角点的坐标集合  
vector<vector<Point2f>> imagePointR;                    //右边摄像机所有照片角点的坐标集合  
vector<vector<Point3f>> objRealPoint;                   //各副图像的角点的实际物理坐标集合   
  
vector<Point2f> cornerL;                              //左边摄像机某一照片角点坐标集合  
vector<Point2f> cornerR;                              //右边摄像机某一照片角点坐标集合  
  
Mat rgbImageL, grayImageL;  
Mat rgbImageR, grayImageR;  
  
Mat cameraMatrixL, cameraDistcoeffL, cameraMatrixR, cameraDistcoeffR ;
vector<Mat> rvecsL, tvecsL,rvecsR, tvecsR;
/*计算标定板上模块的实际物理坐标*/  
void calRealPoint(vector<vector<Point3f>>& obj, int boardwidth, int boardheight, int imgNumber, int squaresize)  
{    
    vector<Point3f> imgpoint;  
    for (int rowIndex = 0; rowIndex < boardheight; rowIndex++)  
    {  
        for (int colIndex = 0; colIndex < boardwidth; colIndex++)  
        {  
            imgpoint.push_back(Point3f(rowIndex * squaresize, colIndex * squaresize, 0));  
        }  
    }  
    for (int imgIndex = 0; imgIndex < imgNumber; imgIndex++)  
    {  
        obj.push_back(imgpoint);  
    }  
}  
  
void outputCameraParam(void)  
{  
    /*保存数据*/  
    /*输出数据*/  
    FileStorage fs("..\\calib_parame\\doubleCameraCalib.xml", FileStorage::WRITE);  
    if (fs.isOpened())  
    {  
        fs << "cameraMatrixL" << cameraMatrixL << "cameraDistcoeffL" << cameraDistcoeffL <<"cameraMatrixR" << cameraMatrixR << "cameraDistcoeffR" << cameraDistcoeffR
		<< "R" << R << "T" << T<< "E" << E << "F" << F;  
        fs.release();   
    }  
    else  
    {  
        cout << "Error: can not save the intrinsics!!!!!" << endl;  
    }  
}  

int main()
{

	 Mat img;  
    int currFrameCount = 0; 
	int goodFrameCount = 0;
    cout << "按Q退出 ..." << endl;  
    while (currFrameCount < frameNumber)  
    {  
        char filename[100];  
        /*读取左边的图像*/  
        sprintf_s(filename, "..\\calib_iamge\\left%02d.jpg", currFrameCount + 1);  
        rgbImageL = imread(filename, CV_LOAD_IMAGE_COLOR);  
        cvtColor(rgbImageL, grayImageL, CV_BGR2GRAY);   
        /*读取右边的图像*/  
        sprintf_s(filename, "..\\calib_iamge\\right%02d.jpg", currFrameCount + 1);  
        rgbImageR = imread(filename, CV_LOAD_IMAGE_COLOR);  
        cvtColor(rgbImageR, grayImageR, CV_BGR2GRAY);  
  
        bool isFindL, isFindR;   
        isFindL = findChessboardCorners(rgbImageL, boardSize, cornerL);  
        isFindR = findChessboardCorners(rgbImageR, boardSize, cornerR);  
        if (isFindL == true && isFindR == true)  //如果两幅图像都找到了所有的角点 则说明这两幅图像是可行的  
        {  
            /* 
            Size(5,5) 搜索窗口的一半大小 
            Size(-1,-1) 死区的一半尺寸 
            TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1)迭代终止条件 
            */  
            cornerSubPix(grayImageL, cornerL, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));  
            drawChessboardCorners(rgbImageL, boardSize, cornerL, isFindL);    
            imagePointL.push_back(cornerL);  
            cornerSubPix(grayImageR, cornerR, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1));  
            drawChessboardCorners(rgbImageR, boardSize, cornerR, isFindR);  
            imagePointR.push_back(cornerR); 
			currFrameCount++;
            goodFrameCount++;  
            cout << "The image is good" << endl;  
        }  
        else  
        {  
            cout << "The image is bad please try again" << endl;
			currFrameCount++;
			continue;
        }  
        if (waitKey(10) == 'q')  
        {  
            break;  
        }  
    }
	/*计算实际的校正点的三维坐标*/  
	calRealPoint(objRealPoint, boardWidth, boardHeight, goodFrameCount, squareSize);  
    cout << "cal real successful" << endl;
	 /* 
    图像采集完毕 接下来开始摄像头的校正 
    calibrateCamera() 
    输入参数 objectPoints  角点的实际物理坐标 
             imagePoints   角点的图像坐标 
             imageSize     图像的大小 
    输出参数 
             cameraMatrix  相机的内参矩阵 
             distCoeffs    相机的畸变参数 
             rvecs         旋转矢量(外参数) 
             tvecs         平移矢量(外参数） 
    */        
	 /*标定摄像头*/  
    calibrateCamera(objRealPoint, imagePointL, Size(imageWidth, imageHeight), cameraMatrixL, cameraDistcoeffL, rvecsL, tvecsL, 0);
	calibrateCamera(objRealPoint, imagePointR, Size(imageWidth, imageHeight), cameraMatrixR, cameraDistcoeffR, rvecsR, tvecsR, 0);
    cout << "calibration successful" << endl;  
    double rms = stereoCalibrate(objRealPoint, imagePointL, imagePointR,  
        cameraMatrixL, cameraDistcoeffL,  
        cameraMatrixR, cameraDistcoeffR,  
        Size(imageWidth, imageHeight), R, T, E, F,  
        CALIB_USE_INTRINSIC_GUESS,  
        TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));  
	cout<<"R="<<R<<endl;
	cout<<"T="<<T<<endl;
	cout << "Stereo Calibration done with RMS error = " << rms << endl;
	outputCameraParam(); 
    system("pause");  
    return 0;  
}

