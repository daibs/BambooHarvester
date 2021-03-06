#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include <math.h>
#include <string>
#include <sstream>      // std::ostringstream
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <WinGDI.h>

using namespace cv;
using namespace std;

template <typename T> std::string tostr(const T& t)
{
	std::ostringstream os; os<<t; return os.str();
}

int main( int argc, char **argv)
{
	HWND desktop;
	RECT rc;
	HDC hdc;
	BITMAPINFO bmpInfo;
	LPDWORD lpPixel;
	HDC hMemDC;
	HBITMAP hBitmap;
	BITMAP bmBitmap;
	int width, height;
	IplImage *convertIplImg;

	// スクリーンの情報を得る
	desktop=GetDesktopWindow();
	GetWindowRect(desktop,&rc);
	width=rc.right;
	height=rc.bottom;

	// DIBの情報を設定する
	bmpInfo.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biWidth=width;
	bmpInfo.bmiHeader.biHeight=height;
	bmpInfo.bmiHeader.biPlanes=1;
	bmpInfo.bmiHeader.biBitCount=32;
	bmpInfo.bmiHeader.biCompression=BI_RGB;

	// DIBSection作成
	hdc=GetDC(desktop);
	hBitmap=CreateDIBSection(hdc,&bmpInfo,DIB_RGB_COLORS,(void**)&lpPixel,NULL,0);
	hMemDC=CreateCompatibleDC(hdc);
	SelectObject(hMemDC,hBitmap);

	// スクリーンをDIBSectionにコピー
	hdc=GetDC(desktop);
	BitBlt(hMemDC,0,0,width,height,hdc,0,0,SRCCOPY);
	ReleaseDC(desktop,hdc);

	// ビットマップ情報の取得
	GetObject(hBitmap, sizeof(BITMAP), &bmBitmap);

	// Bitmap構造体からIplImageへコピー
	convertIplImg = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 4);
	Mat img = Mat(Size(width,height),CV_8UC4);
	memcpy(img.data, bmBitmap.bmBits, width*height*4);
	flip(img,img,0);
	memcpy(convertIplImg->imageData, bmBitmap.bmBits, width*height*4);
	cvFlip(convertIplImg, convertIplImg, 0);

	// OpenCVウィンドウの作成＆画像の表示
	//cvNamedWindow("Desptop Image", CV_WINDOW_AUTOSIZE);
	//cvShowImage("Desptop Image", convertIplImg);
	imshow("desktop",img);
	// 何かキー入力を待つ
	cvWaitKey(0);
	cv::destroyAllWindows();
	// 終了処理
	//cvDestroyWindow("Desptop Image");
	cvReleaseImage(&convertIplImg);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
	PostQuitMessage(0);

	return 0;
}