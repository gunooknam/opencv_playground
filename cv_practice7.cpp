#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize 함수 사용을 위해
#include <iostream>
using namespace std;
using namespace cv;

// 영상을 선명하게 만든다.

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
		const uchar * prev = image.ptr<const uchar>(j - 1); // 이전 행
		const uchar * cur =  image.ptr<const uchar>(j );    // 현재 행
		const uchar * next = image.ptr<const uchar>(j + 1); // 다음 행
		uchar * output = result.ptr<uchar>(j);
		for (int i = nchannels; i < (image.cols - 1)*nchannels; i++) {
			*output++ = saturate_cast<uchar>(5 * cur[i] - cur[i - nchannels] - cur[i + nchannels] - prev[i] - next[i]);
			// 후의 연산이다. 대입되고 포인터증가
		}
		// 필터는 아래와 같음
		// 0 -1 0 
		// -1 5 -1
		// 0 -1 0 
	}
	//row(0).setTo(칼라지정가능)
	result.row(0).setTo(Scalar(0, 0, 0)); // 첫 라인
	result.row(result.rows - 1).setTo(Scalar(0, 0, 0)); // 가장자리 로
	result.col(0).setTo(cv::Scalar(0, 0, 0)); // 첫 칼럼
	result.col(result.cols - 1).setTo(cv::Scalar(0, 0, 0)); // 가장자리 칼럼
}