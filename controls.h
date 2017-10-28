#include "stdafx.h"
#include "wtypes.h"
#include <cv.h>
#include <highgui.h>
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int sensitivity;
int comPort;
bool settingsBtn_Pressed=false;
bool FitToScreen_Pressed=false;
bool reso800x600_Pressed=false;
bool reso640x480_Pressed=false;
bool userManual_Pressed=false;
bool targetBtn_Pressed=false;

Mat settingsBtn=imread("settingsButton.jpg"); 
Mat prevFrame,currentFrame;	//the two frames needed to be compared
int reso_width=800, reso_hight=600;
Mat background(320, 240, CV_8UC3, Scalar(240,240,240));
Mat resolution_FitToDisplay=imread("FitToDisplay.png");
Mat resolution_800x600=imread("resolution_800x600.jpg"); 
Mat resolution_640x480=imread("resolution_640x480.jpg");
Mat userManualBtn=imread("userManual.png");
Mat targetBtn=imread("targets.png");
char TrackbarName[50];

const string trackbarWindowName = "Controls";
void CreateControllers();
void showControls();

void updateSensitivity( int value, void* );
void updateComPort( int value, void* );

void init()
{
	resize(settingsBtn, settingsBtn, Size(35, 35), 0, 0, INTER_LINEAR);
	sensitivity=45;
	comPort=3;
	CreateControllers();
}
void atttachSettingsBtn(Mat &frame)
{	
	settingsBtn.copyTo(frame(Rect(frame.cols-settingsBtn.cols,0,settingsBtn.cols, settingsBtn.rows)));
	//color, thickness, lineType, shift
	rectangle( frame, Point( frame.cols-1, 0 ), Point( frame.cols-settingsBtn.cols, settingsBtn.cols), Scalar( 255, 110, 0 ), 2, 4 );
}

void updateSensitivity( int value, void* )
{
	if(sensitivity==0)
	{
		sensitivity=1;
	}
}
void updateComPort( int value, void* )
{
	if(comPort==0)
	{
		comPort=1;
	}
}
void getDesktopResoltion(int& width, int& height)
{
	RECT desktop;
	//Get handle on Desktop
	HWND desktopHandle = GetDesktopWindow();
	//Get Screen size form desktopHandle
	GetWindowRect(desktopHandle, &desktop);
	width = desktop.right;
	height = desktop.bottom;
}
void setResoFitToDisplay()
{
	getDesktopResoltion(reso_width,reso_hight);
}
void setReso800x600()
{
	reso_width=800;
	reso_hight=600;
}
void setReso640x480()
{
	reso_width=640;
	reso_hight=480;
}
void openUserManual()
{
	ShellExecute(NULL, _T("open"), _T("UserManual.pdf"), NULL, NULL, SW_SHOW);
}
void openTargetFolder()
{
	ShellExecute(NULL, NULL, L"Targets", NULL, NULL, SW_SHOWNORMAL);
}

void controlsBtnFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
    {
		cout<<x<<'\t'<<y<<'\n';
		/*
		if ((x>=20 && x<=70) && (y>=20 && y<60))
        {
			FitToScreen_Pressed=true;
			rectangle( background, Point( 20, 20 ), Point( 70,60), Scalar( 255, 180, 0 ), 2, 4 );
        }
		if ((x>=100 && x<=150) && (y>=20 && y<60))
        {
			reso800x600_Pressed=true;
			rectangle( background, Point( 100, 20 ), Point( 150,60), Scalar( 255, 180, 0 ), 2, 4 );
        }
		if ((x>=180 && x<=230) && (y>=20 && y<60))
        {
			reso640x480_Pressed=true;
			rectangle( background, Point( 180, 20 ), Point( 230,60), Scalar( 255, 180, 0 ), 2, 4 );
        }
		*/
		if ((x>=95 && x<=155) && (y>=80 && y<110))
        {
			userManual_Pressed=true;
			rectangle( background, Point( 95, 80 ), Point( 155,110), Scalar( 255, 180, 0), 2, 4 );
        }
		if ((x>=95 && x<=155) && (y>=130 && y<160))
        {
			targetBtn_Pressed=true;
			rectangle( background, Point( 95, 130 ), Point( 155,160), Scalar( 255, 180, 0), 2, 4 );
        }
    }
    if (event == EVENT_LBUTTONUP)
    {
		/*
		if(FitToScreen_Pressed==true)
		{
			FitToScreen_Pressed=false;
			rectangle( background, Point( 20, 20 ), Point( 70,60), Scalar( 255, 110, 0 ), 2, 4 );
			setResoFitToDisplay();
		}
		if(reso800x600_Pressed==true)
		{
			reso800x600_Pressed=false;
			rectangle( background, Point( 100, 20 ), Point( 150,60), Scalar( 255, 110, 0 ), 2, 4 );
			setReso800x600();
		}
		if(reso640x480_Pressed==true)
		{
			reso640x480_Pressed=false;
			rectangle( background, Point( 180, 20 ), Point( 230,60), Scalar( 255, 110, 0 ), 2, 4 );
			setReso640x480();
		}
		*/
		if(userManual_Pressed==true)
		{
			userManual_Pressed=false;
			rectangle( background, Point( 95, 80 ), Point( 155,110), Scalar( 255, 110, 0), 2, 4 );
			openUserManual();
		}
		if(targetBtn_Pressed==true)
		{
			targetBtn_Pressed=false;
			rectangle( background, Point( 95, 130 ), Point( 155,160), Scalar( 255, 110, 0), 2, 4 );
			openTargetFolder();
		}
    }
	imshow(trackbarWindowName,background);

    waitKey(1);
}

void CreateControllers()	// To Create controllers
{
	//need store trackbar name on window
	
	//creating an empty ioamge.
	
	//adding resolution buttons
	/*
	resize(resolution_FitToDisplay, resolution_FitToDisplay, Size(50, 40), 0, 0, INTER_LINEAR);
	resize(resolution_800x600, resolution_800x600, Size(50, 40), 0, 0, INTER_LINEAR);
	resize(resolution_640x480, resolution_640x480, Size(50, 40), 0, 0, INTER_LINEAR);

	resolution_FitToDisplay.copyTo(background(Rect(20,20,resolution_FitToDisplay.cols, resolution_FitToDisplay.rows)));
	resolution_800x600.copyTo(background(Rect(100,20,resolution_800x600.cols, resolution_800x600.rows)));
	resolution_640x480.copyTo(background(Rect(180,20,resolution_640x480.cols, resolution_640x480.rows)));

	rectangle( background, Point( 20, 20 ), Point( 70,60), Scalar( 255, 110, 0 ), 2, 4 );
	rectangle( background, Point( 100, 20 ), Point( 150,60), Scalar( 255, 110, 0 ), 2, 4 );
	rectangle( background, Point( 180, 20 ), Point( 230,60), Scalar( 255, 110, 0 ), 2, 4 );
	*/
	//adding user Manual button
	resize(userManualBtn, userManualBtn, Size(60, 30), 0, 0, INTER_LINEAR);
	userManualBtn.copyTo(background(Rect(95,80,userManualBtn.cols, userManualBtn.rows)));
	rectangle( background, Point( 95, 80 ), Point( 155,110), Scalar( 255, 110, 0), 2, 4 );

	//adding button to open targets folder
	resize(targetBtn, targetBtn, Size(60, 30), 0, 0, INTER_LINEAR);
	targetBtn.copyTo(background(Rect(95,130,targetBtn.cols, targetBtn.rows)));
	rectangle( background, Point( 95, 130 ), Point( 155,160), Scalar( 255, 110, 0), 2, 4 );

	/*	
	string text = "TQR";
	int fontStyle = CV_FONT_VECTOR0;
	double size = 2;
	int thickness = 1;  
	int lineType=1;
	putText(background, text, Point(0,200), fontStyle, size, Scalar(0,0,0), thickness,lineType);
	*/
}
void showControls()
{
	//create window for trackbars
	namedWindow(trackbarWindowName,CV_WINDOW_NORMAL);
	resizeWindow(trackbarWindowName, 510, 600);

	sprintf_s( TrackbarName, "Sensitivity ", sensitivity);
	sprintf_s( TrackbarName, "compPort ", comPort);
	
	createTrackbar( "Sensitivity ", trackbarWindowName, &sensitivity, 255, updateSensitivity);
    createTrackbar( "compPort ", trackbarWindowName, &comPort, 13, updateComPort );

	imshow(trackbarWindowName,background);
	setMouseCallback(trackbarWindowName, controlsBtnFunc);
}

void settingsBtnFunc(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
    {
		/*
		cout<<x<<'\t'<<y<<'\n'; 
		cout<<prevFrame.cols<<'\t'<<prevFrame.rows<<'\n';
		cout<<prevFrame.cols-settingsBtn.cols<<'\n';
		*/
		if ((x>=prevFrame.cols-settingsBtn.cols && x<=prevFrame.cols) && (y>=0 && y<settingsBtn.rows))
        {
            cout<<"Clicked...\n";
			settingsBtn_Pressed=true;
            rectangle( prevFrame, Point( prevFrame.cols-1, 0 ), Point( prevFrame.cols-settingsBtn.cols, settingsBtn.cols), Scalar( 255, 180, 0 ), 2, 4 );
        }
    }
    if (event == EVENT_LBUTTONUP && settingsBtn_Pressed==true)
    {
        rectangle( prevFrame, Point( prevFrame.cols-1, 0 ), Point( prevFrame.cols-settingsBtn.cols, settingsBtn.cols), Scalar( 255, 110, 0 ), 2, 4 );
		settingsBtn_Pressed=false;
		showControls();
    }
    imshow("Oringal Video Feed", prevFrame);
    waitKey(1);
}
