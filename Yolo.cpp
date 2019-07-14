#include <opencv2/dnn.hpp>             //   이거를 인클루드해야 한다.
#include <opencv2/dnn/shape_utils.hpp> //   이거를 인클루드해야 한다.
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <direct.h>
using namespace std;
using namespace cv;
using namespace cv::dnn;
// git reset HEAD^  // 커밋한거 취소
// OPENCV YOLO  
// => 모델은 darknet형식으로 해야된다는 단점이 있다.
// VOC_dataset 기준으로 진행
int main() {
	char path_str[256];
	String path(_getcwd(path_str, 256));
	// 절대 경로 형태로 변경
	String modelConfiguration = path+"\\yolo_config\\yolov2-voc.cfg";
	String modelBinary        =	path+"\\yolo_config\\yolov2-voc.weights"; // 이 파일은 다크넷 홈페이지가서 다운로드하기

								// 절대패스로 넣어줬음
	Net net = readNetFromDarknet(modelConfiguration, modelBinary);
	vector<String> classNamesVec;

	ifstream classNamesFile("yolo_config/voc.names"); // ifstream을 받아와서

	if (classNamesFile.is_open()) {
		string className = "";
		while (std::getline(classNamesFile, className)) // string으로 바로 넣을 수 있다. 
			classNamesVec.push_back(className); // 자동으로 cv::String으로 형 변환
	}

	VideoCapture cap("vtest.avi");

	while (1) {
		Mat frame;
		cap >> frame; 
		if (frame.empty()) { waitKey(); break; }
		// alpha 채널 있다면??...
		if (frame.channels() == 4) cvtColor(frame, frame, COLOR_BGRA2BGR);
		// Mat을 input들의 batch로 변환
		Mat inputBlob = blobFromImage(frame, 1 / 255.F,
			                          Size(416, 416),
			                          Scalar(),
			                          true, // swap_RG => 왜냐면 opencv 가 RB 바껴있어서 다시 바꿈
			                          false); 
		// set the network input
		net.setInput(inputBlob, "data"); 
		Mat detectionMat = net.forward("detection_out"); // compute output
		// detectionMat의 각의 행은 그 물체가 발견된 확률이 저장된다. 

		// row개 만큼의 detection이 나온다.  
		cout << " Detection Size : " << detectionMat.size() <<" /"                 //  [25 x 845] 가 나온다면
			 << " Detection rows(Count) : " << detectionMat.rows  << " /"          //  총 845개의 rows가 나온다.
			 << " Detection cols(coord: 4, obj_conf:1, class_conf: 20) : " << detectionMat.cols << endl;   //  총  25개의 cols가 나온다.

		float confidenceThreshold = 0.24; // 일단 이걸로 설정
		// 한 row에는 옆에 처럼 데이터가 구성된다. (x_center, y_center, w, h, Obj_Confidence, c1, c2, c3, c4...... ,c20)
		for (int i = 0; i < detectionMat.rows; i++) { // 행을 쭉 검색을 한다.
			// 여기 코드 해석 //
			const int probability_index = 5; 
			//  detectionMat.cols => 은 4 + class index들을 나타낸다. 
			const int probability_size = detectionMat.cols - probability_index; // 이건 그냥 class 개수 인셈
			float * prob_array_ptr = &detectionMat.at<float>(i, probability_index); // 포인터값 블러오거 

			//max_element는 가장 높은 값을 가진 value의 주소값을 리턴한다.  // c1,c2,c3,....c20까지의 가장 높은 class의 확률을 뽑는다.

			size_t objectClass = max_element(prob_array_ptr, prob_array_ptr+probability_size)-prob_array_ptr;
			// 가장 높은 확률 갖는 주소값 - 처음 주소값 = 선택된 클래스 나온다.
			// 주소값 4차인데 타입 바껴서 4->1이 된다
			// 특정한 물체가 detection된 확률, i번째 row에 
			// 해당 Class의 Confidence가 > confidenceThreshold 보다 클 때

			float confidence = detectionMat.at<float>(i , (int)objectClass + probability_index); // 가장 칸 object Class 의 confidece를 비교한다. 
			//For drawing 
			if (confidence > confidenceThreshold) { // 특정한 물체가 존재할 확률과 어떠한 위치에 있는지도 알려준다. 
				float x_center = detectionMat.at<float>(i, 0) * frame.cols;
				float y_center = detectionMat.at<float>(i, 1) * frame.rows;
				float width =    detectionMat.at<float>(i, 2) * frame.cols;
				float height =   detectionMat.at<float>(i, 3) * frame.rows;
				Point p1(cvRound(x_center - width / 2), cvRound(y_center - height / 2));
				Point p2(cvRound(x_center + width / 2), cvRound(y_center + height / 2));
				Rect object(p1, p2); //
				Scalar object_roi_color(0, 255, 0);
				rectangle(frame, object, object_roi_color);
				String className = objectClass < classNamesVec.size() ? classNamesVec[objectClass] : cv::format("unknown(%d)", objectClass);
				String label = format("%s:%.2f", className.c_str(), confidence);
				int baseLine = 0;
				Size labelSize = getTextSize(label, FONT_HERSHEY_SIMPLEX, 0.5 , 1, &baseLine);
				rectangle(frame,
					      Rect(p1, Size(labelSize.width, labelSize.height + baseLine)),
					      object_roi_color,
					      FILLED);
				putText(frame, label, p1 + Point(0, labelSize.height), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
			}
		}
		imshow("YOLO : Detections", frame);
		if (waitKey(1) >= 0) break;
	}
	return 0;
}