// ��������궨.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "opencv.hpp"

using namespace std;
using namespace cv;

const int imageWidth = 640;                             //����ͷ�ķֱ���  
const int imageHeight = 480;  
const int boardWidth = 9;                               //����Ľǵ���Ŀ  
const int boardHeight = 6;                              //����Ľǵ�����  
const int boardCorner = boardWidth * boardHeight;       //�ܵĽǵ�����  
const int frameNumber = 28;                             //����궨ʱ��Ҫ���õ�ͼ��֡��  
const int squareSize = 41;                              //�궨��ڰ׸��ӵĴ�С ��λmm  
const Size boardSize = Size(boardWidth, boardHeight);   //  
Size imageSize = Size(imageWidth, imageHeight);  
  
Mat R, T, E, F;                                         //R ��תʸ�� Tƽ��ʸ�� E�������� F��������    
vector<vector<Point2f>> imagePointL;                    //��������������Ƭ�ǵ�����꼯��  
vector<vector<Point2f>> imagePointR;                    //�ұ������������Ƭ�ǵ�����꼯��  
vector<vector<Point3f>> objRealPoint;                   //����ͼ��Ľǵ��ʵ���������꼯��   
  
vector<Point2f> cornerL;                              //��������ĳһ��Ƭ�ǵ����꼯��  
vector<Point2f> cornerR;                              //�ұ������ĳһ��Ƭ�ǵ����꼯��  
  
Mat rgbImageL, grayImageL;  
Mat rgbImageR, grayImageR;  
  
Mat cameraMatrixL, cameraDistcoeffL, cameraMatrixR, cameraDistcoeffR ;
vector<Mat> rvecsL, tvecsL,rvecsR, tvecsR;
/*����궨����ģ���ʵ����������*/  
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
    /*��������*/  
    /*�������*/  
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
    cout << "��Q�˳� ..." << endl;  
    while (currFrameCount < frameNumber)  
    {  
        char filename[100];  
        /*��ȡ��ߵ�ͼ��*/  
        sprintf_s(filename, "..\\calib_iamge\\left%02d.jpg", currFrameCount + 1);  
        rgbImageL = imread(filename, CV_LOAD_IMAGE_COLOR);  
        cvtColor(rgbImageL, grayImageL, CV_BGR2GRAY);   
        /*��ȡ�ұߵ�ͼ��*/  
        sprintf_s(filename, "..\\calib_iamge\\right%02d.jpg", currFrameCount + 1);  
        rgbImageR = imread(filename, CV_LOAD_IMAGE_COLOR);  
        cvtColor(rgbImageR, grayImageR, CV_BGR2GRAY);  
  
        bool isFindL, isFindR;   
        isFindL = findChessboardCorners(rgbImageL, boardSize, cornerL);  
        isFindR = findChessboardCorners(rgbImageR, boardSize, cornerR);  
        if (isFindL == true && isFindR == true)  //�������ͼ���ҵ������еĽǵ� ��˵��������ͼ���ǿ��е�  
        {  
            /* 
            Size(5,5) �������ڵ�һ���С 
            Size(-1,-1) ������һ��ߴ� 
            TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 20, 0.1)������ֹ���� 
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
	/*����ʵ�ʵ�У�������ά����*/  
	calRealPoint(objRealPoint, boardWidth, boardHeight, goodFrameCount, squareSize);  
    cout << "cal real successful" << endl;
	 /* 
    ͼ��ɼ���� ��������ʼ����ͷ��У�� 
    calibrateCamera() 
    ������� objectPoints  �ǵ��ʵ���������� 
             imagePoints   �ǵ��ͼ������ 
             imageSize     ͼ��Ĵ�С 
    ������� 
             cameraMatrix  ������ڲξ��� 
             distCoeffs    ����Ļ������ 
             rvecs         ��תʸ��(�����) 
             tvecs         ƽ��ʸ��(������� 
    */        
	 /*�궨����ͷ*/  
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

