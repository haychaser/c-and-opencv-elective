#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <typeinfo>
#include "image_tools.h"
#include "detectors.h"

using namespace cv;
using namespace std;

void liquid_level_detection_usecase2() {

	// HSV HSV trackbars
	Mat masked_img, mask, imghsv;
	int hmin = 0, smin = 0, vmin = 0;
	int hmax = 255, smax = 255, vmax = 255;

	Scalar lower(hmin, smin, vmin);
	Scalar upper(hmax, smax, vmax);

	namedWindow("HSV", (640, 200));
	createTrackbar("Hue min", "HSV", &hmin, 180);
	createTrackbar("Hue max", "HSV", &hmax, 180);
	createTrackbar("Saturation min", "HSV", &smin, 255);
	createTrackbar("Saturation max", "HSV", &smax, 255);
	createTrackbar("Value min", "HSV", &vmin, 255);
	createTrackbar("Value max", "HSV", &vmax, 255);

	// BGR trackbars

	int Blue_min = 0, Green_min = 0, Red_min = 0;
	int Blue_max = 59, Green_max = 255, Red_max = 255;

	namedWindow("BGR", (300, 300));
	createTrackbar("Blue min", "BGR", &Blue_min, 255);
	createTrackbar("Blue max", "BGR", &Blue_max, 255);
	createTrackbar("Green min", "BGR", &Green_min, 255);
	createTrackbar("Green max", "BGR", &Green_max, 255);
	createTrackbar("Red min", "BGR", &Red_min, 255);
	createTrackbar("Red max", "BGR", &Red_max, 255);


	// filter and rezise trackbars
	int image_down_scale_factor = 2;
	int canny_high = 252, canny_low = 116; // canny parameters
	//int dilate_kernal // dilate parameters
	//int erode_kernal // erode parameters
	namedWindow("filters", (640, 200));
	//createTrackbar("resize", "filters", &image_down_scale_factor, 4);
	createTrackbar("canny low", "filters", &canny_low, 255);
	createTrackbar("canny high", "filters", &canny_high, 255);

	// load image
	Mat sample_image = imread("C:/lasse/Portfolio/usecase2/usecase2_sample3.jpg");

	// resize
	if (image_down_scale_factor == 0) {
		image_down_scale_factor = 1;
	}
	int width = sample_image.cols;
	int height = sample_image.rows;
	// resize(sample_image, sample_image, Size(width / image_down_scale_factor, height / image_down_scale_factor), INTER_LINEAR);
	resize(sample_image, sample_image, Size(round(width / image_down_scale_factor), round(height / image_down_scale_factor)), INTER_LINEAR);

	imshow("sample_image", sample_image);     // Display frame



	while (1) {
		
		cout << "Width : " << sample_image.size().width << endl;
		cout << "Height: " << sample_image.size().height << endl;
		cout << "Channels: :" << sample_image.channels() << endl;
		//Width: 640
		//Height : 360
		//Channels : : 3

		Mat cropped_image = sample_image(Range(0, 360), Range(200, 300));

		// BGR mask
		Scalar BGR_lower(Blue_min, Green_min, Red_min);
		Scalar BGR_upper(Blue_max, Green_max, Red_max);
		Mat bgr_masked_img, bgr_mask;

		inRange(cropped_image, BGR_lower, BGR_upper, bgr_mask);
		bitwise_and(cropped_image, cropped_image, bgr_masked_img, bgr_mask);
		imshow("bgr_masked_img", bgr_masked_img);

		// HSV mask
		Scalar HSV_lower(hmin, smin, vmin);
		Scalar HSV_upper(hmax, smax, vmax);

		cvtColor(bgr_masked_img, imghsv, COLOR_BGR2HSV);
		//imshow("imghsv", imghsv);

		inRange(imghsv, HSV_lower, HSV_upper, mask);
		//imshow("mask", mask);

		bitwise_and(imghsv, imghsv, masked_img, mask);
		cvtColor(masked_img, masked_img, COLOR_HSV2BGR);
		imshow("Masked HSV image", masked_img);

		//Histogram_color_LS(masked_img);

		// canny and filters
		Mat imgGray, imgBlur, frame_edges, dilated, eroded;
		Mat kernel = getStructuringElement(MORPH_RECT, Size(7, 7));

		cvtColor(masked_img, imgGray, COLOR_BGR2GRAY);
		imshow("imgGray", imgGray);
		
		//Histogram_gray(imgGray);

		GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
		imshow("imgBlur", imgBlur);

		Canny(imgBlur, frame_edges, canny_low, canny_high);
		imshow("frame_edges", frame_edges);

		dilate(frame_edges, dilated, kernel, Point(-1,-1), 4);
		imshow("frame_dilated", dilated);

		erode(dilated, eroded, kernel, Point(-1, -1), 3);
		imshow("eroded", eroded);


		// contours
		vector<vector<Point>>contours;
		vector<Vec4i>hierarchy;
		findContours(eroded, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		for (int i = 0; i < contours.size(); i++) {
			if (contourArea(contours[i]) > 2000) {
				drawContours(cropped_image, contours, i, (0, 0, 255), 1, 8, hierarchy, 0, Point());

				putText(cropped_image,
					"pixel:" + to_string(contourArea(contours[i])),
					Point(300, 100),
					FONT_HERSHEY_SIMPLEX,
					0.6,
					(0, 0, 255),
					2,
					FILLED,
					false);
				cout << "pixel contour" << 
					contourArea(contours[i]) << endl;
				break;
			}
		}
		imshow("contours", cropped_image);

		waitKey(1);

		char c = (char)waitKey(25);
		if (c == 109) {            //if user press m
		}

		c = (char)waitKey(25);
		if (c == 27) {            //  breaks loop, if user press escape
			break;
		}

		c = (char)waitKey(25);
		if (c == 119) {            // writes image to disk if user press w
			// imwrite("C:/lasse/opencv_images/usecase1/sample_original_frame.jpg", frame);
			// imwrite("C:/lasse/opencv_images/usecase1/canny_bottle.jpg", frame_eroded);
			// imwrite("C:/lasse/opencv_images/usecase1/canny_bottle_contoured.jpg", contoured);
		}

	}
}
