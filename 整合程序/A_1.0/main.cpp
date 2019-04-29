/*********************************ͷ�ļ�***********************************/
#include "include.h"

/*********************************������***********************************/
int main()
{
	/*****************************�ֲ�����*********************************/

	CCameraDS m_CamDS;	
	CCameraDS m_CamDS1;	//����ͷ
	
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

	srcPoint[0] = Point2f( 0,0 );			//����
	srcPoint[1] = Point2f( 1279, 0 );		//����
	srcPoint[2] = Point2f( 0, 719);			//����
	srcPoint[3] = Point2f( 1279, 719);		//����

	warpPoint[0] = Point2f( p1x, p1y );
	warpPoint[1] = Point2f( p2x, p2y);
	warpPoint[2] = Point2f( p3x, p3y);
	warpPoint[3] = Point2f( p4x, p4y);

	//��srcPointΪԴ����warpPointΪĿ����󣬵ó�͸�Ӿ���rotMat
	rotMat = getPerspectiveTransform(srcPoint, warpPoint);
	matRot =getRotationMatrix2D(center,angle,scale);

	
	//�򿪵�0������ͷ���ֱ���Ϊ1280*720
	m_CamDS.OpenCamera( 0, false, 1280, 720 );	
	m_CamDS1.OpenCamera(1,false, 640 ,480);

	namedWindow(A_IMAGE); 
	moveWindow(A_IMAGE,-14,-30);
	moveWindow(M_IMAGE,1160,-30);

	while(1)
	{
		IplImage *pFrame = NULL,*mFrame =NULL;

		//������ͷ��ȡ����Ƶ�в���ÿһ֡
		pFrame = m_CamDS.QueryFrame();
		mFrame = m_CamDS1.QueryFrame();

		//��IplImage��ʽ��ͼ��ת��ΪMat��ʽͼ��
		frame = cvarrToMat(pFrame);
		frame1 =cvarrToMat(mFrame);

		warpPerspective(frame1, imageRot1, rotMat1, imageRot1.size());
		dstImage1 = imageRot1(Rect(0,0,200,280));

		//͸�ӱ任��srcImageΪԴͼ��imageRotΪĿ��ͼ��rotMatΪ��ת����imageRot.size()ΪĿ��ͼ��ĳߴ�
		warpPerspective(frame, imageRot, rotMat, imageRot.size());	

		//imshow("imageRot",imageRot);
		warpAffine(imageRot,rotImage,matRot,imageRot.size());
		//����ͼ����
		dstImage = imageProcess();

		//��ʾͼ��
		imshow(A_IMAGE,dstImage);
		imshow(M_IMAGE,dstImage1);
	
		changeMat ();
		//�ȴ�
		if( release == 0)
		{
			m_CamDS.CloseCamera();	
			return 0;
		}
	}
}

/******************************ͼ������*********************************/
Mat imageProcess()
{
	//ͼƬ�ü�����(329,301��Ϊ���϶��㣬�ü����Ϊ620*116��ͼ��
	imageCut = rotImage(Rect( (int)(360-(base*4.2)), 640-base ,(int)base*8.4 ,(int)base*2));
	
	//����ͼƬ��imageCutΪԴͼ��dstImageΪĿ��ͼ��Size(1366,256)Ϊ���ŵ�����ߴ磬CV_INTER_LINEARΪ���Բ�ֵ
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