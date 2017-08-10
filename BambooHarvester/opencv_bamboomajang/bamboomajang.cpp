﻿#include "hai_detector.h"
#include "desktop_capture.h"
#include "yuukouhai.h"
#include "cv_keycode.h"

int main(int argc, char* argv[]) {
	hai_detector det;
	desktop_capture cap;
	int code = 0;
	cv::Mat img;


	//waitKey(0);
	if (argc > 1) {
		img = cv::imread(argv[1], 1);
		cv::cvtColor(img, img, CV_BGR2BGRA);
	}
	cap.capture();
	img = cap.getimg();
	//cv::imshow("capture", img);
	//cv::waitKey(0);
	if (det.gamedetect(img) < 0.3) {
		cout << "Game window could'nt be detected" << endl;
	}
	while (code != CV_WAITKEY_ESC) {
		if (cap.capture() == true && argc == 1) {
			img = cap.getimg();
		}
		det.detect(img);
		det.showpieces();

		int throwhai[38] = { 0 };
		vector<int> result;
		Yuukouhai inst;
		for (int i = 0; i<9; i++) {
			throwhai[i + 21] = det.getpiece(i);
		}
		inst.set_tehai(throwhai);
		if (det.getnum() == 13) {
			//inst.NormalYuukou(&result);
			inst.showYuukou("有効牌:");
		}
		else if (det.getnum() == 14) {
			inst.kiruhai();
		}
		det.showimg();

		//Mat dummy = Mat::zeros(Size(10,10),CV_8UC3);
		//imshow("dummy",dummy);

		code = cvWaitKey(0);
		cout << "code:" << code << endl;
	}
	cv::destroyAllWindows();
	return 0;
}
