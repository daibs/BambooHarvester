#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/contrib/contrib.hpp>
#include <math.h>
#include <string>
#include <sstream>      // std::ostringstream
#include <fstream>
#include <iostream>

using namespace cv;
using namespace std;
template <typename T> std::string tostr(const T& t)
{
    std::ostringstream os; os<<t; return os.str();
}

int main(int argc, char** argv){
	Mat haidata[9];
	double threshold[9] = {0.96,0.98,0.98,0.975,0.95,0.92,0.97,0.97,0.95};
	String hainame = "bamboo_hai_";
	String extension = ".png";
	for(int i = 1; i <= 9; i++){
		String filename = hainame + tostr(i) + extension;
		haidata[i-1] = imread(filename,1);
		if(haidata[i-1].empty() == true){
			cout << "haidata["<<i-1<<"]を読み込めません"<<endl;
			return -1;
		}
		//imshow(filename,haidata[i-1]);
	}
	Mat img;
	if(argc > 1){
		img = imread(argv[1],1);
	}else{
		img = imread("bamboo_hai_all.png",1);
	}
	if(img.empty() == true){
		cout << "対象画像読み込み失敗"<<endl;
		return -1;
	}

	Mat tmp;
	int counter[9] = {0};
	for(int i=0;i<9;i++){
		matchTemplate(img,haidata[i],tmp,CV_TM_CCORR_NORMED);
		Point pmax_pt = Point(0,0);
		for(int j=0;j<4;j++){
			Point max_pt,min_pt;
			double maxval,minval;
			minMaxLoc(tmp,&minval,&maxval,&min_pt,&max_pt);
			
			if(maxval < threshold[i] || abs(pmax_pt.x - max_pt.x) < 30) break;
			pmax_pt = max_pt;
			counter[i] ++;
			cout<<i+1<<","<<j+1<<":"<<max_pt<<":"<<maxval << endl;
			rectangle(img,max_pt,Point(max_pt.x+44,max_pt.y+51),Scalar(255,i*25,0),2);
			for(int x=-22; x<=22; x++){
				for(int y = -26; y <= 25; y++){
					if(max_pt.y+y < 0 || max_pt.y+y > img.rows || max_pt.x+x < 0 || max_pt.x+x > img.cols)
						break;
					tmp.at<int>(max_pt.y+y,max_pt.x+x) = 0;
				}
			}
			
		}
	}
	cout << endl;
	for(int i=0;i<9;i++){
		cout << i+1 << "索:"<<counter[i] << "個" << endl;
	}
	imshow("tmpl",tmp);
	imshow("result",img);
	waitKey(0);
	cv::destroyAllWindows();
	return 0;
}
