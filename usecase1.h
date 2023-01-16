#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <typeinfo>



// bottle detector
void bottle_detector_usecase1() {
	// HSV HSV trackbars
	Mat frame_masked_img, frame_mask, frame_imghsv;
	int hmin = 66, smin = 0, vmin = 50;
	int hmax = 107, smax = 219, vmax = 197;
	int image_down_scale_factor = 4;

	Scalar lower(hmin, smin, vmin);
	Scalar upper(hmax, smax, vmax);

	namedWindow("HSV", (640, 200));
	createTrackbar("Hue min", "HSV", &hmin, 255);
	createTrackbar("Hue max", "HSV", &hmax, 255);
	createTrackbar("Saturation min", "HSV", &smin, 255);
	createTrackbar("Saturation max", "HSV", &smax, 255);
	createTrackbar("Value min", "HSV", &vmin, 255);
	createTrackbar("Value max", "HSV", &vmax, 255);

	// BGR trackbars
	
	int Blue_min = 52, Green_min = 54, Red_min = 17;
	int Blue_max = 194, Green_max = 194, Red_max = 162;

	namedWindow("BGR", (300, 300));
	createTrackbar("Blue min", "BGR", &Blue_min, 255);
	createTrackbar("Blue max", "BGR", &Blue_max, 255);
	createTrackbar("Green min", "BGR", &Green_min, 255);
	createTrackbar("Green max", "BGR", &Green_max, 255);
	createTrackbar("Red min", "BGR", &Red_min, 255);
	createTrackbar("Red max", "BGR", &Red_max, 255);


	// filter trackbars
	int canny_high = 90, canny_low = 12; // canny parameters
	//int dilate_kernal // dilate parameters
	//int erode_kernal // erode parameters
	namedWindow("filters", (640, 200));
	createTrackbar("canny low", "filters", &canny_low, 255);
	createTrackbar("canny high", "filters", &canny_high, 255);

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

		// frame
		imshow("Frame", frame);     // Display frame

		char c = (char)waitKey(25);
		if (c == 119) {            // writes image to disk and breaks, if user press w
			imwrite("C:/lasse/opencv_images/usecase1/sample_original_frame.jpg", frame);
			// imwrite("C:/lasse/opencv_images/usecase1/canny_bottle.jpg", frame_eroded);
			// imwrite("C:/lasse/opencv_images/usecase1/canny_bottle_contoured.jpg", contoured);
		}

		// histogram original frame
		//Histogram_color_LS(frame);

		// BGR mask
		Scalar BGR_lower(Blue_min, Green_min, Red_min);
		Scalar BGR_upper(Blue_max, Green_max, Red_max);
		Mat bgr_masked_img, bgr_mask;

		inRange(frame, BGR_lower, BGR_upper, bgr_mask);
		bitwise_and(frame, frame, bgr_masked_img, bgr_mask);
		imshow("bgr_masked_img", bgr_masked_img);

		// HSV mask
		Scalar HSV_lower(hmin, smin, vmin);
		Scalar HSV_upper(hmax, smax, vmax);

		cvtColor(bgr_masked_img, frame_imghsv, COLOR_BGR2HSV);
		imshow("imghsv", frame_imghsv);

		inRange(frame_imghsv, HSV_lower, HSV_upper, frame_mask);
		imshow("mask", frame_mask);

		bitwise_and(frame_imghsv, frame_imghsv, frame_masked_img, frame_mask);
		cvtColor(frame_masked_img, frame_masked_img, COLOR_HSV2BGR);
		imshow("Masked HSV image", frame_masked_img);

		//Histogram_color_LS(frame_masked_img);

		// canny and filters
		Mat frame_imgGray, frame_imgBlur, frame_edges, frame_dilated, frame_eroded;
		Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));

		cvtColor(frame_masked_img, frame_imgGray, COLOR_BGR2GRAY);
		imshow("imgGray", frame_imgGray);
		// Histogram_gray(frame_imgGray);

		GaussianBlur(frame_imgGray, frame_imgBlur, Size(3, 3), 3, 0);
		imshow("imgBlur", frame_imgBlur);

		Canny(frame_imgBlur, frame_edges, canny_low, canny_high);
		imshow("frame_edges", frame_edges);

		dilate(frame_edges, frame_dilated, kernel);
		imshow("frame_dilated", frame_dilated);

		erode(frame_dilated, frame_eroded, kernel);
		imshow("eroded", frame_eroded);

		// sample template
		Mat image_template = imread("C:/lasse/opencv_images/usecase1/sample_original_frame.jpg");

		// BGR filter
		Mat bgr_masked_template, bgr_mask_template;
		inRange(image_template, BGR_lower, BGR_upper, bgr_mask_template);
		bitwise_and(image_template, image_template, bgr_masked_template, bgr_mask_template);
		imshow("test", bgr_masked_template);

		// HSV filter
		Mat image_template_imghsv, image_template_masked_img, image_template_mask;
		cvtColor(bgr_masked_template, image_template_imghsv, COLOR_BGR2HSV);
		inRange(image_template_imghsv, lower, upper, frame_mask);
		bitwise_and(image_template_imghsv, image_template_imghsv, image_template_masked_img, image_template_mask);
		cvtColor(image_template_masked_img, image_template_masked_img, COLOR_HSV2BGR);

		// canny and filters
		Mat image_template_imgGray, image_template_imgBlur, image_template_edges, image_template_dilated, image_template_eroded;
		cvtColor(image_template_masked_img, image_template_imgGray, COLOR_BGR2GRAY);
		GaussianBlur(image_template_imgGray, image_template_imgBlur, Size(3, 3), 3, 0);
		Canny(image_template_imgBlur, image_template_edges, canny_low, canny_high);
		dilate(image_template_edges, image_template_dilated, kernel);
		erode(image_template_dilated, image_template_eroded, kernel);

		// histogram after HVS mask
		// Histogram_color_LS(masked_img);

		// template contour
		vector<vector<Point>>contours_template;
		vector<Vec4i>hierarchy_template;
		int template_contour_no = 0;

		findContours(image_template_eroded, contours_template, hierarchy_template, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		for (int i_template = 0; i_template < contours_template.size(); i_template++) {
			if (contourArea(contours_template[i_template]) > 55000) {
				//cout << "frame contour number: " << i_template << " pixel size: " << contourArea(contours_template[i_template]) << endl;
				drawContours(image_template, contours_template, i_template, (0, 0, 255), 1, 8, hierarchy_template, 0, Point());
				template_contour_no = i_template;

				putText(image_template,
					"pixel:" + to_string(contourArea(contours_template[i_template])),
					Point(300, 100),
					FONT_HERSHEY_SIMPLEX,
					0.6,
					(0, 0, 255),
					2,
					FILLED,
					false);
				break;
			}
		}
		imshow("template contours", image_template);


		// contours
		// frame contour
		vector<vector<Point>>contours_frame;
		vector<Vec4i>hierarchy_frame;
		int frame_contour_no;
		String passed;

		findContours(frame_eroded, contours_frame, hierarchy_frame, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		for (int i_frame = 0; i_frame < contours_frame.size(); i_frame++) {
			if (contourArea(contours_frame[i_frame]) > 55000) {
				//cout << "frame contour number: " << i_frame << " pixel size: " << contourArea(contours_frame[i_frame]) << endl;
				drawContours(frame, contours_frame, i_frame, (0, 0, 255), 1, 8, hierarchy_frame, 0, Point());
				frame_contour_no = i_frame;

				putText(frame,
					"pixel:" + to_string(contourArea(contours_frame[i_frame])),
					Point(300, 400),
					FONT_HERSHEY_SIMPLEX,
					0.6,
					(255, 180, 180),
					2,
					FILLED,
					false);

				// shape matching frame and template
				double ans = matchShapes(contours_frame[frame_contour_no], contours_template[template_contour_no], CONTOURS_MATCH_I1, 0);
				if (ans < 0.16) {
					passed = "Amicone bottle test passed";
					cout << passed << ": " << ans << " " << endl;
				}
				else {
					passed = "Amicone bottle test failed";
					cout << passed << ": " << ans << " " << endl;
				}
				putText(frame,
					passed,
					Point(300, 100),
					FONT_HERSHEY_SIMPLEX,
					0.6,
					(255, 180, 180),
					2,
					FILLED,
					false);
				break;
			}
		}
		imshow("frame contours", frame);

		waitKey(1);

		// cout << "stream running" << endl;
		c = (char)waitKey(25);
		if (c == 109) {            // match shapes, if user press m
			// matchContoursFull_LS(eroded, image_template);
			// matchContoursFull_usecase1(frame_eroded, image_template_eroded);
		}

		c = (char)waitKey(25);
		if (c == 27) {            // writes image to disk and breaks, if user press escape
			break;
		}
	}
	cap.release();
	//destroyAllWindows();
}


