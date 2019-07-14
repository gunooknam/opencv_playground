#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
#include <time.h>

using namespace cv;
using namespace std;

// Video/ Segmentation
// Chroma-keying
// Surveillance camera �� ���δ�.

// 1 Background subtraction
//  ������ �־����� �������� foreground ��ü Ȯ��

// Code => ����� ���ŵ� ������ ����
// �ٵ� �� �ȵ�

// ���� d(x, y, t) = {  1 or 255  if | f(x, y, ti) - B(x,y, t) | > Threshold
//                         0                  otherwise               }                    

int main() {
	VideoCapture capture("vtest.avi");
	Mat background, image, gray, result, foregroundMask, foregroundImg;
	capture >> background; // background�� ù �������̶� ����
	const int Threshold = 50;
	cvtColor(background, background, CV_BGR2GRAY);
	while (true) {
		// grab()�̶�� �Լ��� ���� �������� �ִ��� Ȯ���Ѵ�.
		if (capture.grab() == 0) break;
		capture.retrieve(image);
		cvtColor(image, gray, CV_BGR2GRAY);
		absdiff(background, gray, foregroundMask); // �� ���� ������ ���밪�� foregroundMask�� ����
		threshold(foregroundMask, foregroundMask, Threshold, 255, CV_THRESH_BINARY); // thresholding���� 
		
		foregroundMask.copyTo(foregroundImg); // ����ũ�� �ϴ� foregroundImg�� �����ϰ�
		gray.copyTo(foregroundImg, foregroundMask); 
		// ���� �̹����� foregroundImg�� �ִµ� Mask �Ⱥκи� ©����.

		imshow("foregroundImg", foregroundImg);
		imshow("foregroundMask", foregroundMask);
		imshow("background", background);
		waitKey(33);
	}
}