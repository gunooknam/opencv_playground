#include <iostream>
#include <opencv2\highgui.hpp>
#include <opencv2\core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp> // cv::cvtColor, cv::remap �Լ� ����� ���� �ʿ�
using namespace cv;
using namespace std;

Mat drawHistogram(Mat src) {

	MatND hist;
	Mat histImage;
	// establish the number of bins
	int i, hist_w, hist_h, bin_w, histSize;

	float range[] = { 0.0, 255.0 };      // pixel range�� ���� 
	const float * histRange = range; // 
	
	int histoGramViewScale = 2;
	hist_w = 256 * histoGramViewScale;
	hist_h = 200 * histoGramViewScale;
	histSize = 256;
	bin_w = cvRound((double)hist_w / histSize);
	
	calcHist(&src,
		        1, // �̹����� ����
				0, // dims ä�� ����Ʈ�̴�. ù �迭�� ä���� 0���� ���� ä���� �ϳ��� 0���� ǥ�� 
		    Mat(), // �ɼ� ����ũ
		     hist, // ��µǴ� ������׷�
		        1, // ������׷� ������ ��
	    &histSize, // �� ������׷��� ������ value ũ�� 
        &histRange);
	
	// histogram image�� ���� �迭 �����.
	histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));

	// normalize the result to [0, histImage.rows] 
	normalize(hist,           //src
		      hist,           //dst
		      0,              //����ȭ�� �ּҰ� 
		      histImage.rows, // ����ȭ�� �ִ� ����
		      NORM_MINMAX,
		      -1,             // ���� ���� �� �̹����� ���� ���̰� ����
		      Mat());

	for (i = 1; i < histSize; i++) {

		line(histImage,
				  Point(bin_w*(i-1),                             // ������ǥ
						hist_h - cvRound(hist.at<float>(i-1)) ), // ���� �Ʒ��ʿ��� hist �� ������ �� ����
			      Point(bin_w*i,                                 // ���� ��ǥ
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