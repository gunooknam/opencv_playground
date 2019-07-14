#include <opencv2/dnn.hpp>             //   �̰Ÿ� ��Ŭ����ؾ� �Ѵ�.
#include <opencv2/dnn/shape_utils.hpp> //   �̰Ÿ� ��Ŭ����ؾ� �Ѵ�.
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <direct.h>
using namespace std;
using namespace cv;
using namespace cv::dnn;
// git reset HEAD^  // Ŀ���Ѱ� ���
// OPENCV YOLO  
// => ���� darknet�������� �ؾߵȴٴ� ������ �ִ�.
// VOC_dataset �������� ����
int main() {
	char path_str[256];
	String path(_getcwd(path_str, 256));
	// ���� ��� ���·� ����
	String modelConfiguration = path+"\\yolo_config\\yolov2-voc.cfg";
	String modelBinary        =	path+"\\yolo_config\\yolov2-voc.weights"; // �� ������ ��ũ�� Ȩ���������� �ٿ�ε��ϱ�

								// �����н��� �־�����
	Net net = readNetFromDarknet(modelConfiguration, modelBinary);
	vector<String> classNamesVec;

	ifstream classNamesFile("yolo_config/voc.names"); // ifstream�� �޾ƿͼ�

	if (classNamesFile.is_open()) {
		string className = "";
		while (std::getline(classNamesFile, className)) // string���� �ٷ� ���� �� �ִ�. 
			classNamesVec.push_back(className); // �ڵ����� cv::String���� �� ��ȯ
	}

	VideoCapture cap("vtest.avi");

	while (1) {
		Mat frame;
		cap >> frame; 
		if (frame.empty()) { waitKey(); break; }
		// alpha ä�� �ִٸ�??...
		if (frame.channels() == 4) cvtColor(frame, frame, COLOR_BGRA2BGR);
		// Mat�� input���� batch�� ��ȯ
		Mat inputBlob = blobFromImage(frame, 1 / 255.F,
			                          Size(416, 416),
			                          Scalar(),
			                          true, // swap_RG => �ֳĸ� opencv �� RB �ٲ��־ �ٽ� �ٲ�
			                          false); 
		// set the network input
		net.setInput(inputBlob, "data"); 
		Mat detectionMat = net.forward("detection_out"); // compute output
		// detectionMat�� ���� ���� �� ��ü�� �߰ߵ� Ȯ���� ����ȴ�. 

		// row�� ��ŭ�� detection�� ���´�.  
		cout << " Detection Size : " << detectionMat.size() <<" /"                 //  [25 x 845] �� ���´ٸ�
			 << " Detection rows(Count) : " << detectionMat.rows  << " /"          //  �� 845���� rows�� ���´�.
			 << " Detection cols(coord: 4, obj_conf:1, class_conf: 20) : " << detectionMat.cols << endl;   //  ��  25���� cols�� ���´�.

		float confidenceThreshold = 0.24; // �ϴ� �̰ɷ� ����
		// �� row���� ���� ó�� �����Ͱ� �����ȴ�. (x_center, y_center, w, h, Obj_Confidence, c1, c2, c3, c4...... ,c20)
		for (int i = 0; i < detectionMat.rows; i++) { // ���� �� �˻��� �Ѵ�.
			// ���� �ڵ� �ؼ� //
			const int probability_index = 5; 
			//  detectionMat.cols => �� 4 + class index���� ��Ÿ����. 
			const int probability_size = detectionMat.cols - probability_index; // �̰� �׳� class ���� �μ�
			float * prob_array_ptr = &detectionMat.at<float>(i, probability_index); // �����Ͱ� ������ 

			//max_element�� ���� ���� ���� ���� value�� �ּҰ��� �����Ѵ�.  // c1,c2,c3,....c20������ ���� ���� class�� Ȯ���� �̴´�.

			size_t objectClass = max_element(prob_array_ptr, prob_array_ptr+probability_size)-prob_array_ptr;
			// ���� ���� Ȯ�� ���� �ּҰ� - ó�� �ּҰ� = ���õ� Ŭ���� ���´�.
			// �ּҰ� 4���ε� Ÿ�� �ٲ��� 4->1�� �ȴ�
			// Ư���� ��ü�� detection�� Ȯ��, i��° row�� 
			// �ش� Class�� Confidence�� > confidenceThreshold ���� Ŭ ��

			float confidence = detectionMat.at<float>(i , (int)objectClass + probability_index); // ���� ĭ object Class �� confidece�� ���Ѵ�. 
			//For drawing 
			if (confidence > confidenceThreshold) { // Ư���� ��ü�� ������ Ȯ���� ��� ��ġ�� �ִ����� �˷��ش�. 
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