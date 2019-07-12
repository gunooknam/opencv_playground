#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize �Լ� ����� ����
#include <iostream>
using namespace std;
using namespace cv;

// ���� �� �÷� ���� ����
// Į�� ������ RGB�� �ϳ��� �ȼ��� ǥ���ȴ�.
// 256 * 256 * 256 = 16,777,216�� ǥ���ȴ�.
// ������ �ڵ�� ǥ���� range�� ���� �� �ִ�.
// ��Ʈ�� ǥ���� range�� ���̴� ��
// 0  ~  7 -> 4
// 8  ~ 15 -> 12
// 16 ~ 23 -> 20
// ...
// 248 ~ 252 => 252


void colorReduce(const Mat & image, Mat&result, int div);

int main() {
	Mat image = imread("hello.jpg");
	Mat result;

	// create �Լ��� ���ο� ũ��� Ÿ������ ������ �ٽ� �Ҵ��� �� ����Ѵ�. 
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

	int num_column = image.cols * image.channels(); // 3ä���� ��츦 ���


	for (int j = 0; j < num_line; j++) {
		const uchar* data_in = image.ptr<uchar>(j);
		uchar * data_out = result.ptr<uchar>(j); 
		// �̰Ž� �����̴�. �״ϱ� �����͸� ���� => ������ �迭�̴ϱ� �ι� ����
		for (int i = 0; i < num_column; i++) {
			data_out[i] = data_in[i] / div * div + div / 2;
		}
	}

}