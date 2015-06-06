/*
* ColorPaddleDetector class
*
* a class which detects motion in a video frame using color. The color
* used for tracking is configured by the user before tracking beings. Motion
* is tracked seperately in the left and right halves of the video frame.
*
*/
#include "ColorPaddleDetector.h"

/*
* ColorPaddleDetector VideoCapture constructor
*
* preconditions:	vid must be a valid VideoCapture object point not equal to
*					nullptr
* postconditions:	sets the left and right paddles equal to the default position,
*					sets this->vid equal to vid, and initilizes the tracking color
*					configurations
*/
ColorPaddleDetector::ColorPaddleDetector(VideoCapture *vid)
{
	m_leftPaddlePos = DEFAULT_PADDLE_POSITION;
	m_rightPaddlePos = DEFAULT_PADDLE_POSITION;
	m_vid = vid;
	configure();
}

/*
* ColorPaddleDetector destructor
*
* preconditions:	none
* postconditions:	deletes this
*/
ColorPaddleDetector::~ColorPaddleDetector() {}

/*
* configure
*
* launches a configurations windows to configure the color to be tracked
*
* preconditions:	none
* postconditions:	sets the values of lowHue, highHue, lowSat, highSat, lowVal, highVal
*					according to the input from the scroll bars in the configuration window
*/
void ColorPaddleDetector::configure()
{

	Mat frame, thresholded;

	namedWindow("Settings", CV_WINDOW_AUTOSIZE); //create a window called "Control"
	//Create trackbars in "Control" window
	cvCreateTrackbar("Low Hue", "Settings", &m_lowHue, 179); //Hue (0 - 179)
	cvCreateTrackbar("High Hue", "Settings", &m_highHue, 179);

	cvCreateTrackbar("Low Saturation", "Settings", &m_lowSat, 255); //Saturation (0 - 255)
	cvCreateTrackbar("High Saturation", "Settings", &m_highSat, 255);

	cvCreateTrackbar("Low Value", "Settings", &m_lowVal, 255); //Value (0 - 255)
	cvCreateTrackbar("High Value", "Settings", &m_highVal, 255);

	while (true)
	{
		*m_vid >> frame;
		flip(frame, frame, 1);
		createThresholdImg(frame, thresholded);
		
		imshow("Configure", thresholded);
		int key = waitKey(30);
		if (key != -1) break;
	}

	// test this
	cvDestroyAllWindows();
}

void ColorPaddleDetector::configureSettings(int e, int x, int y, int flags, void *userData)
{

}

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
void ColorPaddleDetector::createThresholdImg(Mat frame, Mat &dest)
{
	Mat HSV;

	// convert frame from BGR to HSV
	cvtColor(frame, HSV, COLOR_BGR2HSV);

	GaussianBlur(HSV, HSV, Size(7, 7), 2, 2);
	GaussianBlur(HSV, HSV, Size(7, 7), 2, 2);

	// create threshold image using HSV frame and save in dest
	inRange(HSV, Scalar(m_lowHue, m_lowSat, m_lowVal), Scalar(m_highHue, m_highSat, m_highVal), dest);

	
	
}

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
void ColorPaddleDetector::processFrame(Mat &frame)
{
	flip(frame, frame, 1);

	Mat thres;
	createThresholdImg(frame, thres);

	// create left and right threshold images for seperate color detection in
	// left and right sides of the frame
	int x = thres.cols / 2;
	int y = thres.rows;
	Mat thresholdLeft(thres, Rect(0, 0, 320, 480));
	Mat thresholdRight(thres, Rect(320, 0, 320, 480));

	// detect motion in the left and right frames 
	detectMotion(thresholdLeft, frame, IS_RED);
	detectMotion(thresholdRight, frame, IS_BLUE);
}

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
void ColorPaddleDetector::detectMotion(Mat &thres, Mat &frame, bool isRight) {
	
	Moments Moms = moments(thres);

	double m01 = Moms.m01;
	double m10 = Moms.m10;
	double area = Moms.m00;

	if(area > AREA_THRES)
	{
		// calculate the position of the color being tracke in the left frame
		int x = static_cast<int>(m10 / area);
		int y = static_cast<int>(m01 / area);

		Scalar color;
		if(isRight) {
			m_rightPaddlePos = y;
			// position the right object appears in the entire frame
			x = (frame.cols / 2) + x;
			color = BLUE;
		} else {
			m_leftPaddlePos = y;
			color = RED;
		}

		// draw crosshairs through the point being tracked in the left frame
		circle(frame, Point(x, y), 10, color, 2);
		line(frame, Point(x, y + 15), Point(x, y - 15), color, 2);
		line(frame, Point(x + 15, y), Point(x - 15, y), color, 2);
	}
}