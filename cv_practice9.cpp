#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
using namespace cv;
void wave(const cv::Mat &image, cv::Mat &result); // ���� ȿ�� �Լ�
void flip(const cv::Mat &image, cv::Mat &result); // �¿���� �Լ�

int main() {
	Mat before = imread("hello.jpg");
	imshow("before", before);
	cv::Mat after1;
	after1.create(before.rows, before.cols, before.type());
	wave(before, after1); // ���� ȿ��
	cv::imshow("after1", after1);
	cv::Mat after2;
	after2.create(before.rows, before.cols, before.type());
	flip(before, after2); // �¿����
	cv::imshow("after2", after2);
	cv::waitKey(0);
	return 0;
}

// remap �Լ� 
// X ���� �����Ѵ�. matrix ���� (img.rows, img.cols) => �� �ϳ��� ����ִ�.
// Y ���� �����Ѵ�. �� �ϳ��� ����ִ�. 
// remap�� �ϸ� X�ʿ� ������ ���� Y�ʿ� �� ���� �ѽ��� �ȴ�. 
// �� ���� ���� ���� ��ġ�� ������ �ȴ�. 
void wave(const Mat& img, Mat & result) {
	Mat srcX(img.rows, img.cols, CV_32F);
	Mat srcY(img.rows, img.cols, CV_32F);

	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			srcX.at<float>(i, j) = j;
			srcY.at<float>(i, j) = i + 10 * sin(j / 10);
		}
	}
	remap(img,    // �����ó���� ���� x�ʰ� y���� ���� �����ؾ� �Ѵ�. 
		  result, //dst
		  srcX, 
		  srcY,
		  INTER_LINEAR); // interpolation flag
}
void flip(const cv::Mat &img, cv::Mat &result) // �¿� ���� �Լ�
{
	// �� ����
	cv::Mat srcX(img.rows, img.cols, CV_32F);
	cv::Mat srcY(img.rows, img.cols, CV_32F);
	// ���� ����
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			// ȭ�� (i, j)�� ���ο� ��ġ
			srcX.at<float>(i, j) = img.cols - j - 1; // ���� �¿찡 �ٲ� ex) 1��° ���� ������ ����, 2��° ���� (������-1)����,..., ������ ���� 1��° ����.
			srcY.at<float>(i, j) = i; // ���� �״�� ����
		}
	}
	// ���� ����
	cv::remap(img,    // src
		      result, // dst
		      srcX,   
		      srcY,   
		      INTER_LINEAR);

}

