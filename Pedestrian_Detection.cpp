#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요

using namespace cv;
using namespace std;

int main() {
	Mat frame;
	vector<Rect> found;
	int i;
	char ch;
	VideoCapture cap("vtest.avi");

	if (!cap.isOpened()) {
		cout << "can't open video file" << endl;
		return 0;
	}

	HOGDescriptor hog(
		Size(48, 96),
		Size(16, 16),
		Size(8, 8),
		Size(8, 8),
		9
	);

	hog.setSVMDetector(HOGDescriptor::getDaimlerPeopleDetector());
	while (1) {

		// input image
		cap >> frame;
		if (frame.empty()) break;
		
		// detect
		hog.detectMultiScale(
			frame,     // input 이미지
			found,     // 발견된 Rect가 담긴 배열들
			1.2,       // hit Threshold
			Size(8, 8),
			Size(32, 32),
			1.05,
			6);

		//draw results (bounding boxes)
		for (i = 0; i < (int)found.size(); i++) 
				rectangle(frame,
					      found[i], 
					      Scalar(0, 255, 0),
					      2);
		//display
		imshow("Pedestrian Detection", frame);
		ch = waitKey(10);
		if (ch == 27) break;  // ESC Key
		else if (ch == 32)    // SPACE Key
		{  
			while ((ch == waitKey(10)) != 32 && ch != 27);
			if (ch == 27) break;
		}
	}
}