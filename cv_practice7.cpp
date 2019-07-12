#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize �Լ� ����� ����
#include <iostream>
using namespace std;
using namespace cv;

// ������ �����ϰ� �����.

void sharpen(const Mat & image, Mat & result);

int main() {
	Mat before = imread("hello.jpg");
	imshow("before", before);
	Mat after;
	after.create(before.rows, before.cols, before.type());
	sharpen(before, after);
	imshow("After", after);
	waitKey(0);
	return 0;
}


void sharpen(const Mat & image, Mat & result) {
	int nchannels = image.channels();

	for (int j = 1; j < image.rows-1; j++) {
		const uchar * prev = image.ptr<const uchar>(j - 1); // ���� ��
		const uchar * cur =  image.ptr<const uchar>(j );    // ���� ��
		const uchar * next = image.ptr<const uchar>(j + 1); // ���� ��
		uchar * output = result.ptr<uchar>(j);
		for (int i = nchannels; i < (image.cols - 1)*nchannels; i++) {
			*output++ = saturate_cast<uchar>(5 * cur[i] - cur[i - nchannels] - cur[i + nchannels] - prev[i] - next[i]);
			// ���� �����̴�. ���Եǰ� ����������
		}
		// ���ʹ� �Ʒ��� ����
		// 0 -1 0 
		// -1 5 -1
		// 0 -1 0 
	}
	//row(0).setTo(Į����������)
	result.row(0).setTo(Scalar(0, 0, 0)); // ù ����
	result.row(result.rows - 1).setTo(Scalar(0, 0, 0)); // �����ڸ� ��
	result.col(0).setTo(cv::Scalar(0, 0, 0)); // ù Į��
	result.col(result.cols - 1).setTo(cv::Scalar(0, 0, 0)); // �����ڸ� Į��
}