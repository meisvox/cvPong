#include <iostream>
#include <string>
#include "GameBoard.h"
#include "MotionPaddleDetector.h"
#include "ColorPaddleDetector.h"
using namespace std;

/*
* main
* 
* plays a game of cvpong using either color or motion for tracking the paddle
* movements. If no command line arguments were entered, the user is prompted
* for what type of tracking they would like to use: motion or color. 
*
*/
int main(int argc, char *argv[]) {

	PaddleDetector* sherlock;
	GameBoard pong;
	Mat frame;
	string tracking;

	if(argc < 2) {
		// no command line args, prompt for game type
		cout << "Pick your method for motion tracking. Enter \"move\" or \"color\" to play." << endl;
		cout << "tracking: ";
		cin >> tracking;

		if(tracking != CPD_FLAG) {
			tracking = MPD_FLAG;
		}
	} else {
		sscanf_s(argv[1], "%s", &tracking);
	}
	cout << "Gametype = " << tracking;
	cout << " ... initializing game ..." << endl;

	// get videofeed from computer's default camera and set the camer's FPS
	VideoCapture cap(0);
	cap.set(CV_CAP_PROP_FPS, 15);

	// if camera is not on we will exit; cant play without video tracking
	if(!cap.isOpened()) {
		cout << "No camera has been detected, please connect one to play." << endl;
		return(-1);
	}

	if(tracking == CPD_FLAG) {
		sherlock = new ColorPaddleDetector(&cap);
	} else {
		sherlock = new MotionPaddleDetector(&cap);
	}

	while(pong.gameOn()) {
		cap >> frame;
		sherlock->processFrame(frame);
		pong.play(frame, sherlock->getLeftPaddleLoc(), sherlock->getRightPaddleLoc());
		int key = waitKey(30);
		if(key == 27) { break; } // If 'esc' key is pressed we'll quit
	}
	cap.release();

	// hold window until key press
	waitKey(0);
	return(0);
};
