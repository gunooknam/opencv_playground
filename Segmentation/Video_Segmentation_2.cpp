#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
#include <time.h>

using namespace cv;
using namespace std;

// Video/ Segmentation
// Chroma-keying
// Surveillance camera 에 쓰인다.

// 1 Background subtraction
//  비디오가 주어지면 비디오에서 foreground 물체 확인

// Code => 배경이 제거된 영상이 나옴
// 근데 잘 안됨

// 수식 d(x, y, t) = {  1 or 255  if | f(x, y, ti) - B(x,y, t) | > Threshold
//                         0                  otherwise               }                    

int main() {
	VideoCapture capture("vtest.avi");
	Mat background, image, gray, result, foregroundMask, foregroundImg;
	capture >> background; // background를 첫 프레임이라 가정
	const int Threshold = 50;
	cvtColor(background, background, CV_BGR2GRAY);
	while (true) {
		// grab()이라는 함수도 다음 프레임이 있는지 확인한다.
		if (capture.grab() == 0) break;
		capture.retrieve(image);
		cvtColor(image, gray, CV_BGR2GRAY);
		absdiff(background, gray, foregroundMask); // 두 값의 차이의 절대값을 foregroundMask에 저장
		threshold(foregroundMask, foregroundMask, Threshold, 255, CV_THRESH_BINARY); // thresholding으로 
		
		foregroundMask.copyTo(foregroundImg); // 마스크를 일단 foregroundImg에 복사하고
		gray.copyTo(foregroundImg, foregroundMask); 
		// 원본 이미지를 foregroundImg에 넣는데 Mask 된부분만 짤른다.

		imshow("foregroundImg", foregroundImg);
		imshow("foregroundMask", foregroundMask);
		imshow("background", background);
		waitKey(33);
	}
}