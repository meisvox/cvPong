/*
* GameBoard class
*
* a class representing a cvpong gameboard
*
*/
#ifndef GAMEBAORD_H
#define GAMEBOARD_H
#include <time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
using namespace cv;
using namespace std;

const int DEFAULT_X = 640;
const int DEFAULT_Y = 480;
const int BOARDER_WIDTH = 3;
const int BALL_SIZE = 33;
const int PADDLE_X = 18;
const int PADDLE_Y = 120;
const int PADDLE_MOD = PADDLE_Y / 3;
const int WINNING_SCORE = 7;
const double SPEED_INCREMENT = 1.5;

const int MOVE_LEFT = -11;
const int MOVE_RIGHT = 11;
const int MOVE_UP = -11;
const int MOVE_DOWN = 11;
const int MOVE_HORIZ = 0;

const int BOARDER_COLOR[3] = {0, 153, 0};
const int BALL_COLOR[3] = {0, 204, 0};
const int L_PADDLE_COLOR[3] = {0 , 0, 255}; /* red paddle */
const int R_PADDLE_COLOR[3] = {255, 0, 0}; /* blue paddle */


class GameBoard {
public:
	/*
	* GameBoard default constructor
	*
	* preconditions:	none
	* postconditions:	initializes game board to the default values
	*/
	GameBoard();

	/*
	* gameOn
	*
	* returns a boolean representing the status of the game
	*
	* preconditions:	none
	* postconditions:	returns m_gameOn
	*/
	bool gameOn();

	/*
	* play
	*
	* preconditions:	background must be a valid Mat object not equal to nullptr.
	* postconditions:	sets the background image to background. sets the left and right
	*					paddles to the passed in values. displays the gameboard.
	*/
	void play(const Mat& background, int leftPaddlePos, int rightPaddleLoc);

private:
	void displayBall();

	/*
	* setBall
	*
	* draws the ball on the gameboard
	*
	* preconditions:	none
	* postconditions:	sets game ball to the default values
	*/
	void setBall();

	/*
	* checkCollisions
	*
	* determines if the ball has collided with the boundary or a paddle and if so
	* sets the balls movement accordingly. If the ball has collided with an
	* x-boundary than the score is also adjusted accordingly.
	*
	* preconditions:	none
	* postconditions:	sets the ball movement and score if necessary
	*/
	void checkCollisions();

	/*
	* setLeftPaddle
	*
	* preconditions:	none
	* postconditions:	draws the left paddle on the gameboard
	*/
	void setLeftPaddle(int y);

	/*
	* setRightPaddle
	*
	* preconditions:	none
	* postconditions:	draws the right paddle on the gameboard
	*/
	void setRightPaddle(int y);

	/*
	* initPaddles
	*
	* preconditions:	none
	* postconditions:	initializes the paddles to their default locations
	*/
	void initPaddles();

	/*
	* setScore
	*
	* preconditions:	none
	* postconditions:	draws the score on the gameboard and displays the winner when necessary
	*/
	void setScore();

	struct GameBall {
		/*
		* GameBall default constructor
		*
		* preconditions:	none
		* postconditions:	sets game ball to the default values
		*/
		GameBall();

		/*
		* move
		*
		* preconditions:	none
		* postconditions:	sets x-movment and y-movement of the gameball to x and y, respectively
		*/
		void move(double x, double y);

		int m_Xpos;
		int m_Ypos;
		int m_Xmov;
		int m_Ymov;
	};

	struct Paddle {
		int m_Xpos;
		int m_Ypos;
	};

	GameBall m_ball;
	Paddle m_leftPaddle;
	Paddle m_rightPaddle;
	bool m_gameOn;
	int m_score[2];
	double m_speedFactor;
	Mat m_board;
};
#endif