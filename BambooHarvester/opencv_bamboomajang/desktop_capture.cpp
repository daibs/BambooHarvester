#include "desktop_capture.h"

desktop_capture::desktop_capture(void){
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
	img = cv::Mat(cv::Size(width,height),CV_8UC4);
}

bool desktop_capture::capture(void){
	// スクリーンをDIBSectionにコピー
	hdc=GetDC(desktop);
	BitBlt(hMemDC,0,0,width,height,hdc,0,0,SRCCOPY);
	ReleaseDC(desktop,hdc);
	// ビットマップ情報の取得
	GetObject(hBitmap, sizeof(BITMAP), &bmBitmap);
	// Bitmap構造体からMatへコピー
	memcpy(img.data, bmBitmap.bmBits, width*height*4);
	cv::flip(img,img,0);
	if(img.empty() == true)
		return false;
	return true;
}

desktop_capture::~desktop_capture(){
	// 終了処理
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
	PostQuitMessage(0);
}

cv::Mat desktop_capture::getimg(void){	
	return img.clone();
}