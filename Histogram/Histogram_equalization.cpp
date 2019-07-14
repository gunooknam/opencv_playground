#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap 함수 사용을 위해 필요
using namespace cv;
using namespace std;

Mat drawHistogram(Mat src) {

	MatND hist;
	Mat histImage;
	// establish the number of bins
	int i, hist_w, hist_h, bin_w, histSize;

	float range[] = { 0.0, 255.0 };      // pixel range를 정의 
	const float * histRange = range; // 
	
	int histoGramViewScale = 2;
	hist_w = 256 * histoGramViewScale;
	hist_h = 200 * histoGramViewScale;
	histSize = 256;
	bin_w = cvRound((double)hist_w / histSize);
	
	calcHist(&src,
		        1, // 이미지의 개수
				0, // dims 채널 리스트이다. 첫 배열의 채널은 0부터 시작 채널이 하나라서 0으로 표기 
		    Mat(), // 옵션 마스크
		     hist, // 출력되는 히스토그램
		        1, // 히스토그램 차원의 수
	    &histSize, // 한 히스토그램이 가지는 value 크기 
        &histRange);
	
	// histogram image를 담을 배열 만든다.
	histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	// normalize the result to [0, histImage.rows] 
	normalize(hist,           //src
		      hist,           //dst
		      0,              //정규화시 최소값 
		      histImage.rows, // 정규화시 최댓값 설정
		      NORM_MINMAX,
		      -1,             // 음수 값일 때 이미지와 같은 깊이값 취함
		      Mat());

	for (i = 1; i < histSize; i++) {

		line(histImage,
				  Point(bin_w*(i-1),                             // 이전좌표
						hist_h - cvRound(hist.at<float>(i-1)) ), // 제일 아래쪽에서 hist 값 접근한 값 빼기
			      Point(bin_w*i,                                 // 다음 좌표
						hist_h - cvRound(hist.at<float>(i)) ),
			      Scalar(0, 0, 0),
			      2,
			      8,
			      0);
	}
	return histImage;
}

// Does histogram equalization provide an enhanced image ?
// Not always, especially when a certain range of data is dominant
int main() {

	Mat image;
	Mat hist_equalized_image;
	Mat hist_graph;
	Mat hist_equalized_graph;
	image = imread("Mark.png", IMREAD_GRAYSCALE);
	if (!image.data) 
		exit(1); //Check image
	equalizeHist(image, hist_equalized_image); //histogram equlization
	hist_graph = drawHistogram(image);
	hist_equalized_graph = drawHistogram(hist_equalized_image);
	imshow("Input Image", image);

	imshow("Hist Equalized Image", hist_equalized_image);

	imshow("Hist Graph", hist_graph);

	imshow("Hist Equalized Graph", hist_equalized_graph);

	waitKey(0); 
	return 0;

}