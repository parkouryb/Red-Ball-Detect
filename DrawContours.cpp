/*
	BALL DETECT 
*/

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <time.h>
#include <ctime>
#include <ratio>

using namespace std;
using namespace cv;
using namespace std::chrono;

Mat redDetect(const Mat &src) {
	Mat imgHSV, redImg, redImg2, mask;
	cvtColor(src, imgHSV, COLOR_BGR2HSV);

	inRange(imgHSV, Scalar(0, 70, 50), Scalar(10, 255, 255), redImg);
	inRange(imgHSV, Scalar(170, 70, 50), Scalar(180, 255, 255), redImg2);
	//inRange(src, Scalar(90 - 10, 70, 50), Scalar(90 + 10, 255, 255), mask); // Cyan is 90

	return redImg | redImg2;
	//return mask;
}

Mat get_Ball(Mat &src) { // find circle and color : Red
	Mat binary = redDetect(src), finish = src.clone();
	Mat canny_output;
	vector <vector<Point> > contours;
	vector <Vec4i> hierarchy;
	vector <Point> approx;
	Canny(binary, canny_output, 100, 200, 3, 0);
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
	for (size_t i = 0; i < contours.size(); ++i) {
		approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.01, true);
		int areas = contourArea(contours[i], 0);
		if (approx.size() >= 14 && areas > 100) {
			drawContours(finish, contours, i, Scalar(100, 255, 100), 3, 8, hierarchy, 0, Point());
		}
	}
	return finish;
}

int main() {

	Mat src = imread("redd.jpg", 1);
	Mat finish = get_Ball(src);
	imshow("Image", finish);
	/*
	// live video
	VideoCapture capture = VideoCapture(0);
	Mat src, finish;
	namedWindow("Video", WINDOW_NORMAL);
	high_resolution_clock::time_point start = high_resolution_clock::now();
	int frame = 0;

	while (waitKey(27) != 'q') {
		capture >> src;
		++frame;
		finish = get_Ball(src);
		
		// FPS 
		high_resolution_clock::time_point counting = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double>>(counting - start);
		if (time_span.count() >= 1) {
			cout << frame << "\n";
			printf("FPS : %6f\n", (float)frame / time_span.count());
			start = counting;
			frame = 0;
		}

		// show Video

		imshow("Video", finish);
	}
	*/
	waitKey(0);

}