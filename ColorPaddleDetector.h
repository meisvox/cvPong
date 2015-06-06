/*
* ColorPaddleDetector class
*
* a class which detects motion in a video frame using color. The color
* used for tracking is configured by the user before tracking beings. Motion
* is tracked seperately in the left and right halves of the video frame.
*
*/
#pragma once
#include "PaddleDetector.h"
class ColorPaddleDetector :
	public PaddleDetector
{
	const static int AREA_THRES = 10000;

private:

	int m_lowHue = 0;
	int m_highHue = 0;

	int m_lowSat = 0;
	int m_highSat = 0;

	int m_lowVal = 0;
	int m_highVal = 0;

	VideoCapture *m_vid;

	/*
	* configure
	*
	* launches a configurations windows to configure the color to be tracked
	*
	* preconditions:	none
	* postconditions:	sets the values of lowHue, highHue, lowSat, highSat, lowVal, highVal
	*					according to the input from the scroll bars in the configuration window
	*/
	void configure();

	/*
	* createThresholdImg
	*
	* creates a threshold image from frame, removes small objects from the foreground
	* of the thresholded image and then fills the removed holes. The thresholded
	* image is returned in destination.
	*
	* preconditions:	frame must be the frame of video currently being processed.
	* postconditions:	creates a thresholded image from frame and returns it in destination
	*/
	void createThresholdImg(Mat frame, Mat &destination);

	/*
	* detectMotion
	*
	* detects motion in a thresholded image
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

	void configureSettings(int e, int x, int y, int flags, void *userData);

public:
	/*
	* ColorPaddleDetector default constructor
	*
	* preconditions:	none
	* postconditions:	creates ColorPaddleDetector with default settings
	*/
	ColorPaddleDetector() {};

	/*
	* ColorPaddleDetector VideoCapture constructor
	*
	* preconditions:	vid must be a valid VideoCapture object point not equal to
	*					nullptr
	* postconditions:	sets the left and right paddles equal to the default position,
	*					sets this->vid equal to vid, and initilizes the tracking color
	*					configurations
	*/
	ColorPaddleDetector(VideoCapture *vid);

	/*
	* ColorPaddleDetector destructor
	*
	* preconditions:	none
	* postconditions:	deletes this
	*/
	~ColorPaddleDetector();

	/*
	* processFrame
	*
	* creates a threshold image of frame, splits the thresholded image into left and
	* right halves and then tracks for the configured color in each half, setting the
	* left and right paddle positions accordingly
	*
	* preconditions:	frame must be a valid Mat object representing a single frame from
	*					from a VideoCapture object
	* postconditions:	sets left and right paddles according to color detected in the
	*					left and right halves of the frame, respectively
	*/
	void ColorPaddleDetector::processFrame(Mat &frame);
	
};

