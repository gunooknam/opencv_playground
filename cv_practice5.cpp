#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize 함수 사용을 위해
#include <iostream>
using namespace std;
using namespace cv;
#include <stdlib.h> // srand 함수 사용을 위해서
#include <time.h> // time 함수 사용을 위해서

void saltPepperNoise(Mat img, int n) {

	int i, j;
	int salt_or_pepper;
	srand((int)time(NULL));
	for (int k = 0; k < n; k++) {
		i = rand() % img.cols; // x 좌표 0 ~ img.cols-1 까지 랜덤하게 생성
		j = rand() % img.rows; // y 좌표 0 ~ img.rows-1 까지 랜덤하게 생성
		salt_or_pepper = (rand() % 2) * 255; // 0,1 랜덤하게 생성 
		switch (img.type())
		{
		case CV_8UC1:
			img.at<uchar>(j, i) = salt_or_pepper;
			break;
		case CV_8UC3:
			// opencv는 BGR 순으로 들어가 있다.
			// img.at<cv::Vec3b>(j,i) 한덩이로 꺼내면 0,1,2로 접근할 수 있는 배열이다. 
			img.at<cv::Vec3b>(j, i)[0] = salt_or_pepper; // B 채널
			img.at<cv::Vec3b>(j, i)[1] = salt_or_pepper; // G 채널
			img.at<cv::Vec3b>(j, i)[2] = salt_or_pepper; // R 채널
			break;
		default:
			break;
		}
	}

}


int main() {

	Mat img = imread("hello.jpg");
	imshow("before_press_space", img);
	waitKey(0); // space 누르면 다음 창
	saltPepperNoise(img, 5000); // 소금 5000개 뿌리기
	imshow("after", img);
	waitKey(0);
	return 0;
}