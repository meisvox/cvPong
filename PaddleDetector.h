/*
 * PaddleDectector is an abstract class that requires processFrame() to be overridden
 */

#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

static const string MPD_FLAG = "move";
const string CPD_FLAG = "color";

const Scalar RED(0, 0, 255);
const Scalar BLUE(255, 0, 0);
const bool IS_RED = false;
const bool IS_BLUE = true;

/*
* Abstract class PaddleDetector
*
* 
*/
class PaddleDetector
{
	
public:
	
	static const int DEFAULT_PADDLE_POSITION = 0;

	PaddleDetector() {};
	
	virtual ~PaddleDetector() {};

	/*
	 * Abstract method process frame
	 *
	 * Preconditions:	Frame will be a vaild mat object with one frame of video
	 * Postconditions:	Sets the paddle positions of the left and right paddles
	 */
	virtual void processFrame(Mat& frame) = 0;

	/*
	* Abstract get left paddle location
	*
	* Preconditions:	none
	* Postconditions:	returns the location of the left paddle
	*/
	int getLeftPaddleLoc() {return(m_leftPaddlePos);}

	/*
	* Abstract get right paddle location
	*
	* Preconditions:	none
	* Postconditions:	returns the location of the right paddle
	*/
	int getRightPaddleLoc() {return(m_rightPaddlePos);}

protected:
	/*
	* m_leftPaddlePos
	* contains the y-value of the object tracked in the left half of the frame being processed
	*/
	int m_leftPaddlePos;

	/*
	* m_rightPaddlePos
	* contains the y-value of the object tracked in the right half of the frame being processed
	*/
	int m_rightPaddlePos;

private:
	/*
	* Abstract configure
	*
	* Preconditions:	none
	* Postconditions:	configures the tracking setting
	*/
	virtual void configure() {};

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
	virtual void detectMotion(Mat &thres, Mat &frame, bool isRight) = 0;
};

