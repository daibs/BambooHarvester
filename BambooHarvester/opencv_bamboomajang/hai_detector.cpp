#include "hai_detector.h"

hai_detector::hai_detector() {
	double threshold_init[11] = { 0.96,0.98,0.96,0.93,0.94,0.92,0.97,0.95,0.97,0.97,0.95 };
	//	1    2    3    9    5    6	  7    8    4    4	  5
	cv::String hainame = "bamboo_hai_";
	cv::String haimigi = "right_";
	cv::String extension = ".png";
	windowcoord = cv::Point(0, 0);
	for (int i = 0; i < 11; i++) {
		cv::String filename = hainame + tostr(i + 1) + extension;
		try {
			haidata[i] = cv::imread(filename, 1);
		}
		catch (cv::Exception& e) {
			const char* err_msg = e.what();
			cout << "Exception:" << err_msg << endl;
			terminate();
		}
		threshold[i] = threshold_init[i];
		pieces[i] = 0;
	}
	for (int i = 0; i<8; i++) {
		cv::String filename = hainame + haimigi + tostr(i + 1) + extension;
		try {
			hairight[i] = cv::imread(filename, 1);
		}
		catch (cv::Exception& e) {
			const char* err_msg = e.what();
			cout << "Exception:" << err_msg << endl;
			terminate();
		}
	}
	cv::swap(haidata[3], haidata[8]);
	cv::String filename = hainame + "all" + extension;
	try {
		gamewindow = cv::imread(filename, 1);
	}
	catch (cv::Exception& e) {
		const char* err_msg = e.what();
		cout << "Exception:" << err_msg << endl;
		terminate();
	}
	hainum = 0;
}

double hai_detector::gamedetect(cv::Mat tmpimg) {
	cv::cvtColor(tmpimg, tmpimg, CV_BGRA2BGR);
	cout << tmpimg.cols << "," << tmpimg.rows << endl;
	cv::Mat tmp;
	//cv::imshow("tmpimg", tmpimg);
	//cv::imshow("gameimg",gamewindow);
	//cv::waitKey(0);
	cv::matchTemplate(tmpimg, gamewindow, tmp, CV_TM_CCORR_NORMED);
	double maxval;
	minMaxLoc(tmp, NULL, &maxval, NULL, &windowcoord);
	cout << windowcoord.x << "," << windowcoord.y << endl;
	//imwrite("gamewindow.jpg",tmpimg(Rect(windowcoord.x,windowcoord.y,gamewindow.cols,gamewindow.rows)));
	//imshow("gamewindow",img);
	//cout << "gamewindowdetect score:"<<maxval << endl;
	//waitKey(0);
	//cv::destroyAllWindows();

	return maxval;
}


bool hai_detector::detect(cv::Mat src) {
	cv::cvtColor(src, src, CV_BGRA2BGR);
	//if(gamedetect(src) < 0.3){
	//	return false;
	//}
	img = src(cv::Rect(windowcoord.x, windowcoord.y, gamewindow.cols, gamewindow.rows));
	hainum = 0;
	for (int i = 0; i<10; i++) {
		pieces[i] = 0;
	}
	cv::Mat tmp;
	cv::Mat tmpimg = img.clone();

	//cout << img.channels() << "," << img.depth() << endl;
	//cout << haidata[0].channels() << "," << haidata[0].depth() << endl;
	for (int i = 0; i<11; i++) {
		matchTemplate(tmpimg, haidata[i], tmp, CV_TM_CCORR_NORMED);
		cv::Point pmax_pt = cv::Point(0, 0);
		for (int j = 0; j<4; j++) {
			cv::Point max_pt, min_pt;
			double maxval, minval;
			minMaxLoc(tmp, &minval, &maxval, &min_pt, &max_pt);
			if (maxval < threshold[i]) break;
			pmax_pt = max_pt;
			pieces[i] ++;
			cout << i + 1 << "," << j + 1 << ":" << max_pt << ":" << maxval << endl;
			rectangle(img, max_pt, cv::Point(max_pt.x + 44, max_pt.y + 51), cv::Scalar(255, i * 25, 0), 2);
			rectangle(tmpimg, max_pt, cv::Point(max_pt.x + 44, max_pt.y + 51), cv::Scalar(0, 0, 0), CV_FILLED);
			for (int x = -30; x <= 30; x++) {
				for (int y = -5; y <= 5; y++) {
					if (max_pt.y + y < 0 || max_pt.y + y > img.rows || max_pt.x + x < 0 || max_pt.x + x > img.cols)
						break;
					tmp.at<int>(max_pt.y + y, max_pt.x + x) = 0;
				}
			}

		}
		hainum += pieces[i];
	}
	std::swap(pieces[3], pieces[8]);
	pieces[3] += pieces[9];
	pieces[4] += pieces[10];
	for (int i = 0; i<8; i++) {
		matchTemplate(tmpimg, hairight[i], tmp, CV_TM_CCORR_NORMED);
		cv::Point pmax_pt = cv::Point(0, 0);

		cv::Point max_pt, min_pt;
		double maxval, minval;
		minMaxLoc(tmp, &minval, &maxval, &min_pt, &max_pt);
		if (maxval < threshold[i]) continue;
		pmax_pt = max_pt;
		pieces[i] ++;
		hainum++;
		cout << i + 1 << "," << "1" << ":" << max_pt << ":" << maxval << endl;
		rectangle(img, max_pt, cv::Point(max_pt.x + 44, max_pt.y + 51), cv::Scalar(255, i * 25, 0), 2);
		rectangle(tmpimg, max_pt, cv::Point(max_pt.x + 44, max_pt.y + 51), cv::Scalar(0, 0, 0), CV_FILLED);

		break;
	}



	//if(hainum == 14 && pieces[8] > 0){
	//	pieces[8]--;
	//	hainum--;
	//}

	return true;
}

void hai_detector::showpieces(void) {
	cout << endl;
	for (int i = 0; i<9; i++) {
		cout << i + 1 << "索:" << pieces[i] << "個" << endl;
	}
	cout << "牌の個数:" << hainum << endl;
}

int hai_detector::getpiece(int n) {
	return pieces[n];
}

int hai_detector::getnum(void) {
	return hainum;
}

bool hai_detector::showimg(void) {
	if (img.empty() == true)
		return false;
	imshow("img", img);
	//imshow("tmp",tmpimg);
	cv::waitKey(0);
	cv::destroyAllWindows();
	return true;
}