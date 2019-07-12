#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize �Լ� ����� ����
#include <iostream>
using namespace std;
using namespace cv;
#include <stdlib.h> // srand �Լ� ����� ���ؼ�
#include <time.h> // time �Լ� ����� ���ؼ�

void saltPepperNoise(Mat img, int n) {

	int i, j;
	int salt_or_pepper;
	srand((int)time(NULL));
	for (int k = 0; k < n; k++) {
		i = rand() % img.cols; // x ��ǥ 0 ~ img.cols-1 ���� �����ϰ� ����
		j = rand() % img.rows; // y ��ǥ 0 ~ img.rows-1 ���� �����ϰ� ����
		salt_or_pepper = (rand() % 2) * 255; // 0,1 �����ϰ� ���� 
		switch (img.type())
		{
		case CV_8UC1:
			img.at<uchar>(j, i) = salt_or_pepper;
			break;
		case CV_8UC3:
			// opencv�� BGR ������ �� �ִ�.
			// img.at<cv::Vec3b>(j,i) �ѵ��̷� ������ 0,1,2�� ������ �� �ִ� �迭�̴�. 
			img.at<cv::Vec3b>(j, i)[0] = salt_or_pepper; // B ä��
			img.at<cv::Vec3b>(j, i)[1] = salt_or_pepper; // G ä��
			img.at<cv::Vec3b>(j, i)[2] = salt_or_pepper; // R ä��
			break;
		default:
			break;
		}
	}

}


int main() {

	Mat img = imread("hello.jpg");
	imshow("before_press_space", img);
	waitKey(0); // space ������ ���� â
	saltPepperNoise(img, 5000); // �ұ� 5000�� �Ѹ���
	imshow("after", img);
	waitKey(0);
	return 0;
}