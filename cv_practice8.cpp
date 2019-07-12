#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor �Լ��� ����ϱ� ���� �ʿ�. 
using namespace std;
using namespace cv;


int main() {

	Mat img1 = imread("hello.jpg");
	imshow("img1",img1);
	Mat img2 = imread("Mark.png");
	resize(img2, img2, img1.size(), CV_INTER_AREA); // img1 ���·� resize�� ��
	imshow("img2", img2);
	Mat result;
	// �Ʒ� ������� ������ ��� 1
	addWeighted(img1, 0.7,
				img2, 0.9,
		        0.0, // ������ 
		        result); // dst 
	// img * 0.7 + img2 * 0.9
	imshow("Result1",result);
	waitKey(0);
	// ��� 2 
	cv::Mat result1;
	result1 = 0.7*img1+0.9*img2; // broadcast ���� �ȴ�. opearator ���ǵ� \�� 
	cv::imshow("Result2", result1);
	waitKey(0);

	/* �ʷ� ä�ο��� 2���� �̹��� ������ */
	cv::Mat img2_gray;
	cv::Mat result2;
	// img2�� img2_gray�� format �� converting �� �� �ִ�. 
	cv::cvtColor(img2, img2_gray, CV_BGR2GRAY); // �÷� �����̴� img2�� �׷��� �������� ��ȯ. 
	// 1. vector�ε� Mat���¸� ���� ���� �����.
	// 2. Mat ���� 3ä���� -> vector�� split 
	// 3. �� ä�ο� �� ä���� ����. Ÿ���� �⺻���� uint8�̴ϱ� �״�� ������ ��
	std::vector<cv::Mat> planes; // 3�� ������ ���� ����
	cv::split(img1,  //Mat ������ multi channel�� -> Mat�� ������ vector ���·� ��´�.
			  planes); // 3ä���� ���� ������ 3���� 1ä�� �������� �и�
	planes[1] += img2_gray; // �ʷ� ä�ο� ���ϱ�
	cv::merge(planes, result2); // 3���� 1ä�� ������ 3ä���� ���� �������� ����
	cv::imshow("Result2", result2);
	cv::waitKey(0);
	return 0;

}