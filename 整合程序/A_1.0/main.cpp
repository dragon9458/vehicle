/*********************************头文件***********************************/
#include "include.h"

/*********************************主函数***********************************/
int main()
{
	/*****************************局部变量*********************************/

	CCameraDS m_CamDS;	
	CCameraDS m_CamDS1;	//摄像头
	
	if(fs.open("c://data.yml",FileStorage::READ))
	{
		fs["point1x"] >> p1x;
		fs["point1y"] >> p1y;
		fs["point2x"] >> p2x;
		fs["point2y"] >> p2y;
		fs["point3x"] >> p3x;
		fs["point3y"] >> p3y;
		fs["point4x"] >> p4x;
		fs["point4y"] >> p4y;
		fs["angle"] >>angle;
		base = ((int)fs["range"])/2;

		fs.release();
	}
	srcPoint1[0] = Point2f( 0,0 );			
	srcPoint1[1] = Point2f( 639, 0 );		
	srcPoint1[2] = Point2f( 0, 479);			
	srcPoint1[3] = Point2f( 639, 479);
	warpPoint1[0]=Point2f( 0,279 );
	warpPoint1[1]=Point2f( 0,0 );
	warpPoint1[2]=Point2f( 199,279 );
	warpPoint1[3]=Point2f( 199,0 );
	rotMat1 = getPerspectiveTransform(srcPoint1, warpPoint1);
	cout<<rotMat1;

	srcPoint[0] = Point2f( 0,0 );			//左上
	srcPoint[1] = Point2f( 1279, 0 );		//右上
	srcPoint[2] = Point2f( 0, 719);			//左下
	srcPoint[3] = Point2f( 1279, 719);		//右下

	warpPoint[0] = Point2f( p1x, p1y );
	warpPoint[1] = Point2f( p2x, p2y);
	warpPoint[2] = Point2f( p3x, p3y);
	warpPoint[3] = Point2f( p4x, p4y);

	//以srcPoint为源矩阵，warpPoint为目标矩阵，得出透视矩阵rotMat
	rotMat = getPerspectiveTransform(srcPoint, warpPoint);
	matRot =getRotationMatrix2D(center,angle,scale);

	
	//打开第0个摄像头，分辨率为1280*720
	m_CamDS.OpenCamera( 0, false, 1280, 720 );	
	m_CamDS1.OpenCamera(1,false, 640 ,480);

	namedWindow(A_IMAGE); 
	moveWindow(A_IMAGE,-14,-30);
	moveWindow(M_IMAGE,1160,-30);

	while(1)
	{
		IplImage *pFrame = NULL,*mFrame =NULL;

		//从摄像头获取的视频中捕获每一帧
		pFrame = m_CamDS.QueryFrame();
		mFrame = m_CamDS1.QueryFrame();

		//将IplImage格式的图像转换为Mat格式图像
		frame = cvarrToMat(pFrame);
		frame1 =cvarrToMat(mFrame);

		warpPerspective(frame1, imageRot1, rotMat1, imageRot1.size());
		dstImage1 = imageRot1(Rect(0,0,200,280));

		//透视变换，srcImage为源图像，imageRot为目标图像，rotMat为旋转矩阵，imageRot.size()为目标图像的尺寸
		warpPerspective(frame, imageRot, rotMat, imageRot.size());	

		//imshow("imageRot",imageRot);
		warpAffine(imageRot,rotImage,matRot,imageRot.size());
		//进行图像处理
		dstImage = imageProcess();

		//显示图像
		imshow(A_IMAGE,dstImage);
		imshow(M_IMAGE,dstImage1);
	
		changeMat ();
		//等待
		if( release == 0)
		{
			m_CamDS.CloseCamera();	
			return 0;
		}
	}
}

/******************************图像处理函数*********************************/
Mat imageProcess()
{
	//图片裁剪，以(329,301）为左上顶点，裁剪面积为620*116的图像
	imageCut = rotImage(Rect( (int)(360-(base*4.2)), 640-base ,(int)base*8.4 ,(int)base*2));
	
	//缩放图片，imageCut为源图像，dstImage为目标图像，Size(1366,256)为缩放到这个尺寸，CV_INTER_LINEAR为线性插值
	resize(imageCut,dstImage,Size(1146,280),CV_INTER_LINEAR);

	return dstImage;
}

void changeMat (){

	char key =waitKey(1);
	switch (key){
	        case 27:
				release =0 ;
				break;
			case '1':
				flag=1;
				break;
			case '2':
				flag=2;
				break;
			case '3':
				flag=3;
				break;
			case '4':
				flag=4;
				break;
			case '5':
				flag=5;
				break;	
			case '6':
				flag=6;
				break;
			case 'o':		
				fs.open("c://data.yml",FileStorage::WRITE);
				fs<<"point1x"<<p1x
					<<"point1y"<<p1y
					<<"point2x"<<p2x
					<<"point2y"<<p2y
					<<"point3x"<<p3x
					<<"point3y"<<p3y
					<<"point4x"<<p4x
					<<"point4y"<<p4y
					<<"angle"<<angle
					<<"range" << base*2;	
				fs.release();
				break;

			case 'a':
				switch(flag){
					case 1:				
							p1x -= 3;
						warpPoint[0] = Point2f( p1x , p1y);
						rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 2:
							p2x -= 3;
						 warpPoint[1] =Point2f( p2x , p2y);;
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 3:
							p3x -= 3;
						 warpPoint[2] = Point2f( p3x, p3y);
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 4:
							p4x -= 3;
						 warpPoint[3] = Point2f( p4x, p4y);
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 5:
						base+=2;
						break;
					case 6:
						angle+=0.5;
						matRot =getRotationMatrix2D(center,angle,scale);
						break;
					default:
						break;
				}
				break;
			case'd':
				switch(flag){
					case 1:					
							p1x += 3;
						warpPoint[0] = Point2f( p1x , p1y);
						rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 2:
							p2x += 3;
						warpPoint[1] =Point2f( p2x , p2y);;
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 3:
							p3x += 3;
						warpPoint[2] = Point2f( p3x, p3y);
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 4:
							p4x += 3;
						  warpPoint[3] = Point2f( p4x, p4y);
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 5:
						base-=2;
						break;
					case 6:
						angle-=0.5;
						matRot =getRotationMatrix2D(center,angle,scale);
						break;
					default:
						break;
				}
				break;
			case'w':
				switch(flag){
					case 1:
							p1y -= 3;
						warpPoint[0] = Point2f( p1x , p1y);
						rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 2:
							p2y -= 3;
						 warpPoint[1] =Point2f( p2x , p2y);;
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 3:
							p3y -= 3;
						 warpPoint[2] = Point2f( p3x, p3y);
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 4:
							p4y -= 3;
						 warpPoint[3] = Point2f( p4x, p4y);
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;

					default:
						break;
				}
				break;
			case 's':
				switch(flag){
					case 1:
							p1y += 3;
						warpPoint[0] = Point2f( p1x , p1y);
						rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 2:
							p2y += 3;
						warpPoint[1] =Point2f( p2x , p2y);;
						rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 3:
							p3y += 3;
						warpPoint[2] = Point2f( p3x, p3y);
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					case 4:
							p4y += 3;
						 warpPoint[3] = Point2f( p4x, p4y);
						 rotMat = getPerspectiveTransform(srcPoint, warpPoint);
						break;
					default:
						break;
				}
		
				break;
		}		
	}