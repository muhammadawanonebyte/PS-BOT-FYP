#include "stdafx.h"
#include "controls.h"

//To save center of target object
int Target_X_Axis = 320;	
int Target_Y_Axis = 240;
int Prev_Target_X_Axis = 0;	
int Prev_Target_Y_Axis = 0;
bool targetDetected=false;
int midx;
int midy;
int Fcount=0;
Rect targetBoundry = Rect(0,0,0,0);	//To add rectangle boundry accross the object and it center will be the target position
HANDLE hSerial = CreateFile(TEXT("COM3"),  GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
char buff[]="";
DWORD btsIO;
int xDegree=90;
int yDegree=90;

void DrawTargetPosition(Mat &cameraFeed)
{
	//draw some crosshairs around the object
	if(Prev_Target_X_Axis!=Target_X_Axis  ||  Prev_Target_Y_Axis!=Target_Y_Axis)
	{
		targetDetected==true;
	}
	else
	{
		targetDetected=false;
	}
	Prev_Target_X_Axis = Target_X_Axis;	
	Prev_Target_Y_Axis = Target_Y_Axis;
	//img, Point, radius, Scalar(R, G, B), thickness
	circle(cameraFeed, Point(Target_X_Axis, Target_Y_Axis), 10, Scalar(0, 0, 255), 2);
	//img, Point-1, Point-2 radius, Scalar(R, G, B), thickness
	line(cameraFeed, Point(Target_X_Axis, Target_Y_Axis-5), Point(Target_X_Axis, Target_Y_Axis+5), Scalar(0, 255, 0), 1);
	line(cameraFeed, Point(Target_X_Axis-5, Target_Y_Axis), Point(Target_X_Axis+5, Target_Y_Axis), Scalar(0, 255, 0), 1);

	String posTxt="("+to_string(Target_X_Axis)+","+to_string(Target_Y_Axis)+")";

	//write the position of the object to the screen
	//img, String, Point, fontStyle, fontSize, Scalar(R, G, B), thickness=1
	putText(cameraFeed, posTxt, Point(Target_X_Axis+10, Target_Y_Axis+5), 1, 1, Scalar(0, 255, 0),1);
}

//cameraFeed is takken by refference because we want disply the position in actual video feed in main()
void findingAxis(Mat thresholdImage)
{
	Mat temp=thresholdImage;
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );

	//if contours vector is not empty means a target is detected.
	if(contours.size()>0)
	{
		//If multiple targets are detected that the largest target will be our priority
		//Therefore finding the largest contour
		vector< vector<Point> > largTargetContour;
		largTargetContour.push_back(contours.at(contours.size()-1));
		//make a bounding rectangle around the largest contour then find its centroid
		targetBoundry = boundingRect(largTargetContour.at(0));
		Target_X_Axis = targetBoundry.x+targetBoundry.width/2;
		Target_Y_Axis = targetBoundry.y+targetBoundry.height/2;
	}
}
/*
///xxxx
1 deg = 640/27 = 23.7 px
450px/23.7=18.98
18.98-13=v
90+v

///yyyy
1 deg = 480/27 = 17.778 px
260px/17.778=14.625
*/
void sendParameter()
{
	int x=Target_X_Axis/23.7;
	int y=Target_Y_Axis/17.778;
	if(Target_X_Axis!=320 && Target_Y_Axis!=240)
	{
		cout<<Target_X_Axis<<'\t'<<Target_Y_Axis<<'\n';
		if(x<13)
		{
			buff[0]='X';
			buff[1]=char(x+90);
			cout<<"X="<<90+x<<'\n';
			WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
		}
		else if(x>13)
		{
			x=x-13;
			buff[0]='X';
			buff[1]=char(90-x);
			cout<<"X="<<90-x<<'\n';
			WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
		}
		if(y<13)
		{
			buff[0]='Y';
			buff[1]=char(y+90);
			cout<<"Y="<<90+y<<'\n';
			WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
		}
		else if(y>13)
		{
			x=x-13;
			buff[0]='Y';
			buff[1]=char(90-y);
			cout<<"Y="<<90-y<<'\n';
			WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
		}
	
		if(x!=13 || y!=13)
		{
			buff[0]='F';
			WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
			buff[0]='R';
			WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
		
			Target_X_Axis=prevFrame.cols/2;
			Target_Y_Axis=prevFrame.rows/2;
		}
	}
	/*
	midx=prevFrame.cols/2;
	midy=prevFrame.rows/2;
	cout<<midx<<'\t'<<midy<<'\t'<<Target_X_Axis<<'\t'<<Target_Y_Axis<<'\t'<<xDegree<<'\t'<<yDegree<<'\n';
	//if(!((midx<Target_X_Axis+5 && midx>Target_X_Axis-5) && (midx<Target_Y_Axis+5 && midx>Target_Y_Axis-5)))
	{
		if(xDegree<135 && xDegree>45)
		{
			if(Target_X_Axis<midx)
			{
				
				cout<<"Cond true x+\n";
				buff[0]='X';
				xDegree++;
				buff[1]=char(xDegree);
				WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
			}
			else if(Target_X_Axis>midx)
			{
				cout<<"Cond true x-\n";
				buff[0]='X';
				xDegree--;
				buff[1]=char(xDegree);
				WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
			}
		}
		else
		{
			cout<<"X-limit reached\n";
		}
		if(yDegree<110 && yDegree>70)
		{
			if(Target_Y_Axis<midy)
			{
				cout<<"Cond true y+\n";
				buff[0]='Y';
				yDegree++;
				buff[1]=char(yDegree);
				WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
			}
			else if(Target_Y_Axis>midy)
			{
				cout<<"Cond true y-\n";
				buff[0]='Y';
				yDegree--;
				buff[1]=char(yDegree);
				WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
			}
		}
		else
		{
			cout<"Y-limit reached\n";
		}

	}
	if((midx<=Target_X_Axis+5 && midx>Target_X_Axis-5) && (midy<=Target_Y_Axis+5 && midy>Target_Y_Axis-5) && targetDetected==true)
	{
		cout<<"FIRE cond true\n";
		buff[0]='F';
		WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
		buff[0]='R';
		WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);
		xDegree=90;
		yDegree=90;
	}
	*/
}
void main()
{
	bool targetDetected = false;

	init();
	Mat mainFrameGrey,currentFrameGrey; //required grey images to find difference
	Mat targetImage;	//To store result of difference
	Mat targetThreshold;	//To use in function findContours() we need thresholded difference of grey images
	// Setup serial port connection and needed variables.
	
	if (hSerial !=INVALID_HANDLE_VALUE)
    {
        printf("Port opened successfully! \n");

        DCB dcbSerialParams;
        GetCommState(hSerial,&dcbSerialParams);

        dcbSerialParams.BaudRate = CBR_9600;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.Parity = NOPARITY;
        dcbSerialParams.StopBits = ONESTOPBIT;

        SetCommState(hSerial, &dcbSerialParams);
	}
	else
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("Serial port doesn't exist! \n");
		}
		printf("Error while setting up serial port! \n");
	}
	buff[0]='R';
	WriteFile(hSerial, buff, strlen(buff), &btsIO, NULL);

	VideoCapture capture;	//Capture frames from video.
	
	capture.open(0);	//To start camera and open video feed

	if(!capture.isOpened())
	{
		cout<<"ERROR! UNABLE TO READ VIDEO FEED\n";
		system("pause");
		exit(0);
	}
	namedWindow("Oringal Video Feed",CV_WINDOW_NORMAL);
	setMouseCallback("Oringal Video Feed", settingsBtnFunc);
		
	while(1)
	{
		capture.read(prevFrame);	//capture prev frame

		cvtColor(prevFrame, mainFrameGrey, COLOR_BGR2GRAY);	//converting prevFrame to grey

		capture.read(currentFrame);	//capture currentFrame

		cvtColor(currentFrame, currentFrameGrey, COLOR_BGR2GRAY);	//converting currentFrame to grey

		absdiff(mainFrameGrey, currentFrameGrey, targetImage);	//finding difference in frames and saving in sequential image

		threshold(targetImage, targetThreshold, 25, 255, THRESH_BINARY);	//findind threshold of targetImage
		
		blur(targetThreshold,targetThreshold,Size(sensitivity,sensitivity));	//increasing size of target object to remove dots and to smooth intensity

		threshold(targetThreshold, targetThreshold, 25, 255, THRESH_BINARY); //findinding new threshold of blurred targetImage
		
		
		DrawTargetPosition(prevFrame);
		if(Target_X_Axis==320  &&  Target_Y_Axis==240 && Fcount==0)
		{
				findingAxis(targetThreshold);
		}
		else
		{
				Fcount++;
				if(Fcount==1)
				{
					sendParameter();
				}
				if(Fcount>10)
				{
					Fcount=0;
				}
		}
//		imshow("Targeted Threshold Image",targetThreshold);	//showing the finalized targeted threshold image in new window
		resize(prevFrame, prevFrame, Size(1024, 768), 0, 0, INTER_LINEAR); // Resizing to desired size.
		atttachSettingsBtn(prevFrame);
		imshow("Oringal Video Feed",prevFrame); //showing orignal frame in new window

		waitKey(5); //this 10ms delay is required to refresh frames otherwise blank image will apear.
			//Sleep doesn't work
	}
	system("pause");
}