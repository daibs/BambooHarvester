#ifndef __HAI_DETECTOR
#define __HAI_DETECTOR

//#include "opencv2/core/core.hpp"
//#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/contrib/contrib.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>
#include <sstream>      // std::ostringstream


//using namespace cv;
using namespace std;
template <typename T> std::string tostr(const T& t)
{
	std::ostringstream os; os << t; return os.str();
}

class hai_detector {
private:
	cv::Mat img;
	cv::Mat gamewindow;
	cv::Mat haidata[11];
	cv::Mat hairight[8];
	cv::Point windowcoord;
	double threshold[11];
	int pieces[11];
	int hainum;



public:
	hai_detector(void);
	bool detect(cv::Mat);
	void showpieces(void);
	int getpiece(int);
	int getnum(void);
	bool showimg(void);
	double gamedetect(cv::Mat);

};

#endif
