/*
* MotionPaddleDetector class
*
* a class which detects motion in a video frame using sequential images. Motion
* is tracked seperately in the left and right halves of the video frame.
*
*/
#ifndef MOTIONPADDLEDETECTOR_CPP
#define MOTIONPADDLEDETECTOR_CPP
#include "MotionPaddleDetector.h"

/*
* MotionPaddleDetector default constructor
*
* preconditions:	vid must be a valid VideoCapture object point not equal to nullptr
* postconditions:	sets left and right paddles to default position and sets m_vid 
*					to vid
*/
MotionPaddleDetector::MotionPaddleDetector(VideoCapture* vid) : PaddleDetector() {
	m_leftPaddlePos = DEFAULT_PADDLE_POSITION;
	m_rightPaddlePos = DEFAULT_PADDLE_POSITION;
	m_vid = vid;
}

/*
* processFrame
*
* uses sequential images to detect motion in the left and right halves of the frame.
*
* preconditions:	frame must be a valid Mat object representing a single frame from 
*					from a VideoCapture object
* postconditions:	sets left and right paddles according to motion detected in the
*					left and right halves of the frame, respectively
*/
void MotionPaddleDetector::processFrame(Mat& frame) {
	Mat frame2, gray, gray2, thres, diff;

	// use sequential images (frame and frame2) for motion detection

	// read in frame and convert to grayscale
	m_vid->read(frame);
	flip(frame, frame, 1);
	cvtColor(frame, gray, COLOR_BGR2GRAY);

	// read in frame2 and convert to grayscale
	m_vid->read(frame2);
	flip(frame2, frame2, 1);
	cvtColor(frame2, gray2, COLOR_BGR2GRAY);

	// create difference image of frame1 and frame2 after being converted to
	// grayscale images
	absdiff(gray, gray2, diff);

	// threshold difference
	threshold(diff, thres, THRESHOLD_SENSITIVITY, 255, THRESH_BINARY);

	// blur the image. output will be an intensity image
	blur(thres, thres, cv::Size(BLUR_SIZE, BLUR_SIZE));

	// threshold intensity image to get binary image (after blurring)
	threshold(thres, thres, THRESHOLD_SENSITIVITY, 255, THRESH_BINARY);

	// split threshold (now binary image) into left and right halves
	int x = thres.cols / 2;
	int y = thres.rows;
	Mat thresholdLeft(thres, Rect(0, 0, x, y));
	Mat thresholdRight(thres, Rect(x, 0, x, y));

	// detect motion in each half of the binary image
	detectMotion(thresholdLeft, frame, IS_RED);
	detectMotion(thresholdRight, frame, IS_BLUE);
}

/*
* detectMotion
*
* detects motion in a thresholded image by finding all contours in the image and then
* using the largest contour to determine the motion of the paddle.
*
* preconditions:	thres must be one half (left or right) of the threshold image of the
*					difference image from the sequential frames. frame must be the video
*					frame being processed. isRight should be set true if we are detecting
*					motion in the right frame, otherwise it should be false as we are
*					tracking motion in the left frame.
* postconditions:	sets the paddle position of the paddle indicated by isRight and draws
*					a crosshair around the object being tracked
*/
void MotionPaddleDetector::detectMotion(Mat &thres, Mat &frame, bool isRight) {
	bool objectDetected = false;

	// the contour vector and hieracrchy returned from findContours
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	// find contours in binary image
	findContours(thres, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);// retrieves external contours

	// if contours vector is empty, no objects were detected
	objectDetected = contours.size() > 0 ? true : false;

	if(objectDetected){
		// select the largest contour
		vector<vector<Point>> largestContourVec;
		largestContourVec.push_back(contours.at(contours.size() - 1));

		// create a bounding rectangle around the largest contour and then take
		// the center of the bounding rectangle and use this point for tracking
		Rect objBoundingRect = boundingRect(largestContourVec.at(0));
		int x = objBoundingRect.x + objBoundingRect.width / 2;
		int y = objBoundingRect.y + objBoundingRect.height / 2;
		
		Scalar color;
		if(isRight) {
			//update right paddle's position and set crosshair color to blue
			m_rightPaddlePos = y;
			x = (frame.cols / 2) + x;
			color = BLUE;
		} else {
			// update left paddle's position and set crosshair color to red
			m_leftPaddlePos = y;
			color = RED;
		}

		// draw crosshairs through the point being tracked in the frame
		circle(frame, Point(x, y), 10, color, 2);
		line(frame, Point(x, y + 15), Point(x, y - 15), color, 2);
		line(frame, Point(x + 15, y), Point(x - 15, y), color, 2);
	}
}

#endif