#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
using namespace cv;
using namespace std;

int main() {

	CascadeClassifier face_classifier;
	// ��ġ�� => opencv\sources\data\haarcascades��
	face_classifier.load("haarcascade_frontalface_alt.xml");



}