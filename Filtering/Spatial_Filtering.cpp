#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
#include <time.h>

using namespace cv;
using namespace std;

// �پ��� ���͸��� ����

Mat saltPepperNoise(Mat img, int n) {
	int i, j;
	int salt_or_pepper;
	Mat result=img.clone();
	// result.create(img.rows, img.cols, img.type());
	srand((int)time(NULL));
	for (int k = 0; k < n; k++) {
		i = rand() % img.cols; // x ��ǥ 0 ~ img.cols-1 ���� �����ϰ� ����
		j = rand() % img.rows; // y ��ǥ 0 ~ img.rows-1 ���� �����ϰ� ����
		salt_or_pepper = (rand() % 2) * 255; // 0,1 �����ϰ� ���� 
		switch (img.type())
		{
		case CV_8UC1:
			result.at<uchar>(j, i) = salt_or_pepper;
			break;
		case CV_8UC3:
			// opencv�� BGR ������ �� �ִ�.
			// img.at<cv::Vec3b>(j,i) �ѵ��̷� ������ 0,1,2�� ������ �� �ִ� �迭�̴�. 
			result.at<cv::Vec3b>(j, i)[0] = salt_or_pepper; // B ä��
			result.at<cv::Vec3b>(j, i)[1] = salt_or_pepper; // G ä��
			result.at<cv::Vec3b>(j, i)[2] = salt_or_pepper; // R ä��
			break;
		default:
			break;
		}
	}
	return result;
}

int main() {
	Mat image,
		avgImg,
		gaussinImg,
		laplacian,
		abs_laplacian,
		sharperning;

	image = imread("hello.jpg", IMREAD_GRAYSCALE);

	// Blurs an image using the normalized box filter
	// average filter�̴�. 
	blur(image,      //src
		avgImg,      //dst
		Size(5, 5)); // kernel size 5,5


	// ����þ� ����
	GaussianBlur(image,      //src
				 gaussinImg, //dst
		         Size(5, 5), // kernel size 
		         1.5   //sigmaX �̴�. => Gaussian kernel standard deviation in X direction
				);     // sigmaY=0 

	Laplacian(image, laplacian,
		       CV_16S, //dst�� ���ϴ� bit_depth
		       5,      // kernel size�̴�. ���� 3, 5, 7 �̷� ���� ������ ���
		       1,      // scale ��-> �Ի�� �̺а��� ����
		       0);     // delta�� -> ���� �̺а��� �߰�

	convertScaleAbs(laplacian, abs_laplacian);
	sharperning = abs_laplacian + image;

	Mat peppers = saltPepperNoise(image, 15000);
	Mat median_filter1, median_filter2, threshImage,cannyEdge, invCannyEdge;
	medianBlur(peppers, median_filter1, 3);
	medianBlur(peppers, median_filter2, 9);
	
	threshold(
		image,
		threshImage,
		120, // �̷��� �Ӱ谪 ���� ������
		255,  // 255�� ����
		THRESH_BINARY // binary threshold�� �ۿ�
	);
	
	Canny(image, 
		  cannyEdge,
		  100,  // low threshold
		  180); // high threshold

	threshold(cannyEdge,
		      invCannyEdge,
		      125,
		      255,
		      THRESH_BINARY_INV); // INV�ϱ� 125���� ������ 0���� ����


	Mat blur, grad_x, grad_y, abs_grad_x, abs_grad_y, result;
	GaussianBlur(image,
				 blur,
		         Size(5, 5),
		         5,
		         5,
		         BORDER_DEFAULT);

	Sobel(blur, grad_x, CV_16S, 1, 0, 3);
	convertScaleAbs(grad_x, abs_grad_x);

	Sobel(blur, grad_y, CV_16S, 1, 0, 3);
	convertScaleAbs(grad_y, abs_grad_y);

	addWeighted(
		abs_grad_x, 0.5,
		abs_grad_y, 0.5,
		0, result);

	imshow("input Image", image);
	imshow("Average image", avgImg);
	imshow("Gaussian blurred image", gaussinImg);
	imshow("laplacian", laplacian);
	imshow("abs_laplacian", abs_laplacian);
	imshow("sharpening", sharperning);
	imshow("peppers", peppers);
	imshow("filter size 3", median_filter1);
	imshow("filter size 9", median_filter2);
	imshow("threshold", threshImage);
	imshow("cannyEdge", cannyEdge);
	imshow("invCannyEdge", invCannyEdge);
	imshow("SobelEdge", result);

	waitKey(0);
	return 0;

}