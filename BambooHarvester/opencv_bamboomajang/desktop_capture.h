#ifndef __CV_DESKTOP_CAPTURE
#define __CV_DESKTOP_CAPTURE

#include "opencv2/core/core.hpp"
#include <string>
#include <Windows.h>
#include <WinGDI.h>

class desktop_capture{
private:
	cv::Mat img;
	HWND desktop;
	RECT rc;
	HDC hdc;
	BITMAPINFO bmpInfo;
	LPDWORD lpPixel;
	HDC hMemDC;
	HBITMAP hBitmap;
	BITMAP bmBitmap;
	int width, height;

public:
	desktop_capture(void);
	~desktop_capture();
	bool capture(void);
	cv::Mat getimg(void);
	
};

#endif