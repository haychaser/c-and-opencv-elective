#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <typeinfo>
#include "keypoint_matching.h"
#include <opencv2/features2d.hpp>

Mat sift_matching_usecase3(Mat img1, Mat img2) {
	Ptr<SIFT> detector = SIFT::create();
	vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2, img_matches;

	detector->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
	detector->detectAndCompute(img2, noArray(), keypoints2, descriptors2);

	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
	vector<vector<DMatch>> knn_matches;
	matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);

	//const float ratio_thresh = 0.7f;
	const float ratio_thresh = 0.23f;
	vector<DMatch> good_matches;
	for (size_t i = 0; i < knn_matches.size(); i++)
	{
		if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
		{
			good_matches.push_back(knn_matches[i][0]);
		}
	}
	cout << "Of the SIFT keypoints, " << good_matches.size() << "matches were found after filtering with KNN" << endl;

	drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches, Scalar::all(-1),
		Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	imshow("Good Matches", img_matches);
	waitKey(1);
	if (good_matches.size() > 95) {
		cout << "lable ok" << endl;
	}
	else
	{
		cout << "defekt label" << endl;
	}
	return img_matches;


}

void SIFT_video_lable_check_usecase3() {
	
	VideoCapture cap(1);
	if (!cap.isOpened()) {
		cout << "Error opening video stream or file" << endl;
	}
	while (1) {
		Mat frame, frame_hsv_masked, frame_processed;
		cap >> frame;               // Capture frame-by-frame
		if (frame.empty()) {        // Break if no frame
			cout << "frame empty" << endl;
			break;
		}
		
		char c = (char)waitKey(25);
		if (c == 119) {            // writes image to disk and breaks, if user press w
			imwrite("C:/lasse/Portfolio/usecase3/usecase3_template.jpg", frame);
		}

		Mat image_template = imread("C:/lasse/Portfolio/usecase3/usecase3_template.jpg");
		
		sift_matching_usecase3(image_template, frame);

		c = (char)waitKey(25);
		if (c == 109) {            // if user press m
		}

		c = (char)waitKey(25);
		if (c == 27) {            // breaks while loop, if user press escape
			break;
		}
	}
	cap.release();
	destroyAllWindows();
}
