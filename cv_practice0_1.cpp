#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp> // resize 함수 사용을 위해
#include <iostream>

using namespace std;
using namespace cv;

// Pixel access 
// image.at<DATA_TYPE>(row, col) 이렇게 접근한다. 

int main() {
	Mat image, image_gray;
	int value, value_B, value_G, value_R, channels;
	MatIterator_ <uchar> it, end;
	MatIterator_<Vec3b> it3, end3;
	image = imread("hello.jpg",IMREAD_COLOR);
	imshow("Original image", image);
	waitKey(0);
	image_gray = imread("hello.jpg", IMREAD_GRAYSCALE);
	// try both image & image_gray 
	// channels = image_gray.channels();
	channels = image.channels();
	//At operator 
	switch (channels) {
		case 1:
			value = image.at<uchar>(50, 100);
			cout << "value: " << value;
			for (it = image.begin<uchar>(), image.end<uchar>(); it != end; ++it) {
				value = *it; cout << "value: " << value << endl;
			}
			break;
		case 3: // 3 채널은 BGR 순으로 저장되어 있다. 
			cout << "-----배열방식접근-----" << endl;
			value_B = image.at<Vec3b>(50, 100)[0];
			value_G = image.at<Vec3b>(50, 100)[1]; 
			value_R = image.at<Vec3b>(50, 100)[2];
			cout << "value at (100,50) (50행의 100번째 줄): " << value_B << " " << value_G << " " << value_R << endl;
			cout << "----포인터방식접근-----"<< endl;
			uchar* p;
			p = image.ptr<uchar>(50);
			value_B = p[100 * channels + 0]; 
			value_G = p[100 * channels + 1];
			value_R = p[100 * channels + 2];
			cout << "value at (100,50) (50행의 100번째 줄): " << value_B << " " << value_G << " " << value_R << endl;

			cout << "----data를 불러와서 접근----" << endl;
			uchar* data = (uchar*)image.data; // 이렇게 불러오면 그냥 1차원 배열 같은 느낌이다.
			value_B = data[(50 * image.cols + 100) * channels + 0];
			value_G = data[(50 * image.cols + 100) * channels + 1];
			value_R = data[(50 * image.cols + 100) * channels + 2];
			cout << "value at (100,50): " << value_B << " " << value_G << " " << value_R << endl;
			cout << "------MatIterator-------" << endl;
			// begin<data_type>() 이렇게 하면 시작점의 iteration를 반환한다. iterator는 포인터 형식이라 값에 접근은 *을 쓰고 [ ]을 쓴다.
			for (it3 = image.begin<Vec3b>(), end3 = image.end<Vec3b>(); it3 != end3; ++it3) {
				value_B = (*it3)[0];
				value_G = (*it3)[1];
				value_R = (*it3)[2];
				cout << "B: " << value_B << ", G: " << value_G << ", R: " << value_R << endl;
			}
			break;
	}	 
	waitKey(0); 

}