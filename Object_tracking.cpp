#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <direct.h>
using namespace std;
using namespace cv;

struct feature {
	Point2f pt;
	int val;
};

bool initialization = false;
void DrawTrackingPoints(vector<Point2f> &points, Mat &image);

int main(int argc, char* argv[]) {

	VideoCapture cap(0);
	if (!cap.isOpened()) {
	
	}

}