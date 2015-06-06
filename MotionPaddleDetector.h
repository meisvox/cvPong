/*
* MotionPaddleDetector class
*
* a class which detects motion in a video frame using sequential images. Motion
* is tracked seperately in the left and right halves of the video frame.
*
*/
#ifndef MOTIONPADDLEDETECTOR_H
#define MOTIONPADDLEDETECTOR_H
#include "PaddleDetector.h"

class MotionPaddleDetector : public PaddleDetector {
	static const int THRESHOLD_SENSITIVITY = 20;
	static const int BLUR_SIZE = 10;
public:
	/*
	* MotionPaddleDetector default constructor
	*
	* preconditions:	vid must be a valid VideoCapture object point not equal to nullptr
	* postconditions:	sets left and right paddles to default position and sets m_vid
	*					to vid
	*/
	MotionPaddleDetector(VideoCapture* vid);

	/*
	* MotionPaddleDetector destructor
	*
	* preconditions:	none
	* postconditions:	none
	*/
	~MotionPaddleDetector() {}

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
	virtual void processFrame(Mat& frame);

private:
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
	void detectMotion(Mat &thres, Mat &frame, bool isRight);

	VideoCapture* m_vid;
};

#endif