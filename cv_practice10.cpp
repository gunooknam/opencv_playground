#include "opencv2\opencv.hpp"
using namespace cv;
using namespace std;

int main()
{
	VideoCapture Video;		 // => ( 비디오를 읽기 위한 클래스 )
	VideoWriter outputVideo; // => ( 비디오를 저장하기 위한 클래스 )
	Mat frame;

	Video.open("vtest.avi");
	if (!Video.isOpened())
	{
		printf("Cannot open a video file\n");
		return -1;
	}

	double fps = Video.get(CV_CAP_PROP_FPS);
	Size size = Size((int)Video.get(CAP_PROP_FRAME_WIDTH), (int)Video.get(CAP_PROP_FRAME_HEIGHT));
	cout <<"fps :" << fps << endl;
	cout <<"size : " << size << endl;
	

	Video.grab();
	Video.retrieve(frame);

	outputVideo.open("new_vtest.avi", 
					 CV_FOURCC('M', 'P', '4', 'V'),
		             30.f, 
				     Size(frame.cols, frame.rows), 
		             true);

	if (!outputVideo.isOpened()) {
		return -1;
	}

	while(1)
	{
		Video.grab();
		bool read = Video.retrieve(frame);
		if (!read) break;
		// 비디오 프레임의 현재 위치
		size_t pos = Video.get(CV_CAP_PROP_POS_FRAMES);

		cout << pos << endl;

		imshow("frame", frame);

		outputVideo.write(frame);
		waitKey(30);
	}
	
	waitKey(0);
	Video.release(); //
	outputVideo.release(); // 

	return 0;
}



