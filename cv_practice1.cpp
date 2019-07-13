#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize �Լ� ����� ����
#include <iostream>

using namespace std;
using namespace cv;

// �̹��� �ø� �� ������
// waitKey��...? Ű�� ��ٸ��� �Լ��̴�.
// ���ϰ����� �ش� Ű�� ���� ���� ���� ���Ϲ޴´�.
// SPCAE 32,
// ESC 27 ���� ���ڸ� ���Ϲ޴´�.

int main() {
	Mat image = imread("hello.jpg");
	if (image.empty()) {
		cout << "error when reading image!" << endl;
		return 0;
	}
	cout << "Original image is ( " << image.rows << " ," << image.cols << ")" << endl;	
	// �� �Ʒ� �ΰ� �̸��� �ٸ��� â �ΰ��� �߰� ������ 1�����. 
	namedWindow("Original image"); // ���� �̹����� ������ â ����� 
	imshow("Original image", image);

	Mat resized_image;
	cout << Size() << endl; // �̰��� [0 x 0]
	cv::resize(image, resized_image, Size(), 0.5, 0.5, CV_INTER_AREA);

	cout << "Resized image is " << resized_image.rows << "x" << resized_image.cols << endl;
	namedWindow("Resized image");
	imshow("Resized image", resized_image);

	Mat result;
	flip(resized_image, result, 1); // flip�̶�� �Լ��� �ִ�. src, dst, ��� ������ ���ΰ�? 

	namedWindow("Filed image");
	imshow("Filed image", result);
	imwrite("Filped image.jpg", result);
	waitKey(0);
	destroyAllWindows();
	return 0;
}
