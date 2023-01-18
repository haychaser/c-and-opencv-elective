#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <typeinfo>
#include "usecase1v0002.h"
#include "usecase2v0001.h"
#include "usecase3v0001.h"
#include <opencv2/core/utils/logger.hpp>

using namespace cv;
using namespace std;

int main(){
	utils::logging::setLogLevel(utils::logging::LogLevel::LOG_LEVEL_SILENT);

	//test();
	// usecase1
	//bottle_detector_usecase1();


	// usecase 2
	liquid_level_detection_usecase2();

	// usecase 3
	//SIFT_video_lable_check_usecase3();



	
}
