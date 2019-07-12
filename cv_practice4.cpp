#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize �Լ� ����� ����
#include <iostream>
using namespace std;
using namespace cv;

// �̹����� �ΰ� �ִ� ����
int main() {
	Mat bonobono = imread("hello.jpg");
	Mat logo = imread("Mark.png");
	Mat logo_gray = imread("Mark.png", CV_LOAD_IMAGE_GRAYSCALE);
	cout << "image size:" << bonobono.size() << endl;
	cout << "logo size:" << logo.size() << endl;


	// Rect�� x, y , w, h �̴�. 
	Mat imageROI(bonobono, // �ش� src �̹����� 
				 Rect(bonobono.cols - logo.cols, // ���� ��ǥ�� �ش��ϴ� patch�� ��´�.
					  bonobono.rows - logo.rows,
					  logo.cols,
					  logo.rows ));
	

	Mat mask(120 - logo_gray); 
	// logo gray�� CV_8U�̱� ������ �̰� 120 �� ���� ���� CV_8U��
	// clip�� �ȴ�. ������ �ȳ���
	cout << mask.size() << imageROI.size() << endl;
	// 120 - logo_gray�� ���ָ� �� ��� �κ��� �� 0�� �ȴ�. => Ǫ
	// logo�� ī���Ѵ� 
	logo.copyTo(imageROI, // dst
				mask);    // ����ũ ���� 0�� �ƴ� ��ġ������ �ΰ� ���Եȴ�.
						  // ����ũ�� �ι�° ���ڷ� ������
						  // ����ŷ �ǹ����� 0�� �ƴ� ���� ������.
	
	imshow("image", bonobono); // �ΰ� �־��� ���뺸��
	// ���� ImageROI�� ���� ���Ӱ� ���簡 �Ȱ��� �ƴϴ�. ���� ������ �� ���̴�.
	imageROI = 0;
	imshow("image_ref_zero", bonobono); // �������� Ȯ��
	
	waitKey(0);

	return 0;
}