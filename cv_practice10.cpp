#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
using namespace cv;
using namespace std;

int main() {

	CascadeClassifier face_classifier;
	// 위치는 => opencv\sources\data\haarcascades”
	face_classifier.load("haarcascade_frontalface_alt.xml");



}