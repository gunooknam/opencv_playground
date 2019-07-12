#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize 함수 사용을 위해
#include <iostream>
using namespace std;
using namespace cv;

// 영상 내 컬러 개수 감소
// 칼라 영상은 RGB가 하나의 픽셀로 표현된다.
// 256 * 256 * 256 = 16,777,216이 표현된다.
// 다음의 코드로 표현의 range를 줄일 수 있다.
// 비트의 표현의 range를 줄이는 것
// 0  ~  7 -> 4
// 8  ~ 15 -> 12
// 16 ~ 23 -> 20
// ...
// 248 ~ 252 => 252


void colorReduce(const Mat & image, Mat&result, int div);

int main() {
	Mat image = imread("hello.jpg");
	Mat result;

	// create 함수는 새로운 크기와 타입으로 영상을 다시 할당할 때 사용한다. 
	result.create(image.rows, image.cols, image.type());
	colorReduce(image, result, 32);
	imshow("before", image);
	waitKey(0);
	imshow("after", result);
	waitKey(0);
	return 0;
}


void colorReduce(const Mat & image, Mat&result, int div) {
	int num_line = image.rows;
	cout << "image channel" << image.channels() << endl;

	int num_column = image.cols * image.channels(); // 3채널일 경우를 대비


	for (int j = 0; j < num_line; j++) {
		const uchar* data_in = image.ptr<uchar>(j);
		uchar * data_out = result.ptr<uchar>(j); 
		// 이거슨 참조이다. 그니까 포인터만 참조 => 이차원 배열이니까 두번 참조
		for (int i = 0; i < num_column; i++) {
			data_out[i] = data_in[i] / div * div + div / 2;
		}
	}

}