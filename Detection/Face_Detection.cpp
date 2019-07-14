#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
using namespace cv;
using namespace std;
int main() {

	CascadeClassifier face_classifier;
	Mat frame, grayframe;
	vector <Rect> faces;
	int i;
	// open the webcam
	VideoCapture cap(0); // webcam을 연다.
	if (!cap.isOpened()) { // 웹캠을 열자 // check if we succeeded
		cout << "Could not open camera" << endl;
		return -1;
	 } // face detection configuration
	face_classifier.load("haarcascade_frontalface_alt.xml"); 

	while (true) {
		cap >> frame; // get a new frame from webcam
		// convert captured frame to gray scale
		cvtColor(frame, grayframe, COLOR_BGR2GRAY);
		// grayscale로 넣는다. 

		face_classifier.detectMultiScale(
			grayframe,
			faces,
			1.1,
			3,
			0,
			Size(30, 30) // min size=> 최소 객체 사이즈
		);
		//  @param flags Parameter with the same meaning for an old cascade as in the function
		//  cvHaarDetectObjects.It is not used for a new cascade.
		// draw the results 
		for (int i = 0; i < faces.size(); i++) {
			Point lb(faces[i].x + faces[i].width,
				     faces[i].y + faces[i].height);
			Point tr(faces[i].x,
					 faces[i].y);
			rectangle(frame,
				      lb,
				      tr,
				      Scalar(0, 255, 0),
				      3, 4, 0);
		}

		// print the output
		imshow("Face Detection", frame);
		if (waitKey(33) == 27) break;
	}
}