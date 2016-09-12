#include "myLib.h"
#include "text.h"
#include <stdio.h>
#include <string.h>
#include "leftBorder.h"
#include "rightBorder.h"
#include "title.h"
#include "ball.h"
#include "endScreen.h"
#include "levelScreen.h"

int main()
{
	//Initialize Mode 3
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	//Initialize paddle, ball, and brick array
	PADDLE paddle = {120 - PADDLEWIDTH / 2, 0, 120 - PADDLEWIDTH / 2, DARKBLUE };
	BALL ball = {STARTINGLOCR, STARTINGLOCC, STARTINGLOCR, STARTINGLOCC, BALLSIZE, BALLCOLOR, 1, 1};
	BRICK bricks[80];

	// Level outlines
	int level1[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	int level2[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					0, 1, 0, 0, 1, 0, 2, 2, 2, 0, 
					0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 
					0, 2, 2, 2, 2, 0, 0, 1, 0, 0, 
					0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 
					0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 
					0, 1, 0, 0, 1, 0, 2, 2, 2, 0, 
					0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int level3[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
					 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 2, 2, 2, 2, 2, 2, 2, 1, 1, 1,
					 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
					 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 2, 2, 2, 2, 2, 2, 2, 1, 1, 1};

	int level4[] =  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
					 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
					 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
					 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	int level5[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					1, 0, 2, 2, 0, 0, 2, 2, 0, 1,
					1, 0, 2, 2, 0, 0, 2, 2, 0, 1,
					1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
					1, 2, 0, 0, 0, 0, 0, 0, 2, 1,
					1, 0, 2, 0, 0, 0, 0, 2, 0, 1,
					1, 0, 0, 2, 2, 2, 2, 0, 0, 1,
					1, 1, 1, 1, 1, 1, 1, 1, 1, 1};



	
	// Instantiating the levels	
	LEVEL l1 = {1, level1, 40};
	LEVEL l2 = {2, level2, 24};
	LEVEL l3 = {3, level3, 40};
	LEVEL l4 = {4, level4, 50};
	LEVEL l5 = {5, level5, 48};

	//Start state
	enum GBAState state = TITLE;

	// Instantiate game state variables
	int score = 0;
	int level = 1;
	int newLevel = 1;
	int bricksHit = 0;
	LEVEL curLevel = l1;
	int lives = STARTINGLIVES;

	// Create strings for display
	char scoreStr[15] = "Score";
	char livesStr[15] = "Lives";
	char levelStr[15] = "Level";
	char scoreStrVal[8];
	char livesStrVal[1];
	char levelStrVal[1];
	
	// Game Loop
	while(1)
	{	
		// Wait for Vblank call before each loop
		waitForVblank();

		switch(state) {

		// Starting state
		case TITLE:

			// Draw title screen
			drawImage3(0,0,TITLE_WIDTH,TITLE_HEIGHT, title_data);
			drawString(100, 88, "PRESS START", ORANGE);
			state = TITLE_NODRAW;
			break;

		// Resets data for game restarts and waits for button input
		case TITLE_NODRAW:

			ball.r = STARTINGLOCR;
			ball.c = STARTINGLOCC;

			
			ball.oldR = STARTINGLOCR;
			ball.oldC = STARTINGLOCC;

			ball.rMov = 1;
			ball.cMov = 1;

			paddle.c = 120 - PADDLEWIDTH / 2;
			paddle.oldC = 120 - PADDLEWIDTH / 2;

			bricksHit = 0;
			level = 1;
			sprintf(levelStrVal, "%d", level);
			curLevel = l1;
			score = 0;
			lives = 3;

			// Takes the level outlines from above and turns them into bricks
			brickMaker(curLevel.levelBricks, bricks);

			while(KEY_DOWN_NOW(BUTTON_START) || KEY_DOWN_NOW(BUTTON_A) || KEY_DOWN_NOW(BUTTON_SELECT));
			while(!KEY_DOWN_NOW(BUTTON_START) && !KEY_DOWN_NOW(BUTTON_A));

			state = LEVELSTATE;
			break;

		// Begins displays and awaits button input to start the game
		case GAMESETUP:

			// Check to see if its game over
			if (!lives) {
				state = LOSE;
				break;
			} else {
				if (newLevel) {
					newLevel = 0;
					bricksHit = 0;	
					brickMaker(curLevel.levelBricks, bricks);
				}
			}

			//Resets data, dispays game screen, awaits input
			ball.r = STARTINGLOCR;
			ball.c = STARTINGLOCC;
			ball.oldR = STARTINGLOCR;
			ball.oldC = STARTINGLOCC;
			ball.rMov = 1;
			ball.cMov = 1;

			paddle.c = 120 - PADDLEWIDTH / 2;
			paddle.oldC = 120 - PADDLEWIDTH / 2;

			fillScreen(BGCOLOR);

			drawImage3(0,0, LEFTBORDER_WIDTH, LEFTBORDER_HEIGHT, leftBorder_data);
			drawImage3(0,199, RIGHTBORDER_WIDTH, RIGHTBORDER_HEIGHT, rightBorder_data);

			drawString(66, 5, scoreStr, ORANGE);
			drawString(125, 5, livesStr, ORANGE);
			drawString(14, 5, levelStr, ORANGE);

			drawImage3(ball.r, ball.c, BALL_HEIGHT, BALL_WIDTH, ball_data);
			drawRect(155, paddle.c, PADDLEHEIGHT, PADDLEWIDTH, BLUE);
			drawRect(156, paddle.c + 1, PADDLEHEIGHT - 2, PADDLEWIDTH - 2, DARKBLUE);

			for (int i = 0; i < NUMBRICKSROW*NUMBRICKSCOL; i++) {
				BRICK *a = &bricks[i];
				unsigned short color = TEAL;
				unsigned short border = TBOR;
				if (a->hits == 2) {
					color = DARKGREEN;
					border = DGBOR;
				} else if (a->hits == 3) {
					color = DARKPURPLE;
					border = PBOR;
				}
				drawRect(a->r, a->c, a->h, a->w, border);
				drawRect(a->r + 1, a->c + 1, a->h - 2, a->w - 2, color);

			}

			sprintf(scoreStrVal, "%d", score);
			drawRect(85, 7, 10, 26, BLACK);
			drawString(87, 8, scoreStrVal, ORANGE);

			sprintf(livesStrVal, "%d", lives);
			drawRect(142, 16, 10, 8, BLACK);
			drawString(144, 17, livesStrVal, ORANGE);

			sprintf(levelStrVal, "%d", level);
			drawRect(31, 16, 10, 8, BLACK);
			drawString(33, 17, levelStrVal, ORANGE);

			while(KEY_DOWN_NOW(BUTTON_START) || KEY_DOWN_NOW(BUTTON_A));
			while(!KEY_DOWN_NOW(BUTTON_A) && !KEY_DOWN_NOW(BUTTON_START) && !KEY_DOWN_NOW(BUTTON_SELECT));
			if (KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = TITLE;
			} else {
				state = GAME;
			}
			break;
		case GAME:

			// Updates score, lives, and level
			sprintf(scoreStrVal, "%d", score);
			drawRect(85, 7, 10, 26, BLACK);
			drawString(87, 8, scoreStrVal, ORANGE);

			sprintf(livesStrVal, "%d", lives);
			drawRect(142, 16, 10, 8, BLACK);
			drawString(144, 17, livesStrVal, ORANGE);

			sprintf(levelStrVal, "%d", level);
			drawRect(31, 16, 10, 8, BLACK);
			drawString(33, 17, levelStrVal, ORANGE);
			
			// Right button moves paddle right, left button to the left
			if(KEY_DOWN_NOW(BUTTON_RIGHT)) {
				paddle.c += 2;
				paddle.cMov = 1;
				if (paddle.c > 239 - 40 - PADDLEWIDTH) {
					paddle.c -= 2;
					paddle.cMov = 0;
				}
				
			} else if(KEY_DOWN_NOW(BUTTON_LEFT)) {
				paddle.c -= 2;
				paddle.cMov = -1;
				if (paddle.c < 41) {
					paddle.c += 2;
					paddle.cMov = 0;
				}
			} else {
				paddle.cMov = 0;
			}

			// Up button can influence ball movement to make finishing easier/debugging
			if(KEY_DOWN_NOW(BUTTON_UP))  {
				ball.r -= ball.rMov;
			}

			// Select resets back to title screen
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = TITLE;
				break;
			}

			// Ball moves and checks its bounds
			ball.r += ball.rMov;
			ball.c += ball.cMov;
			int dead = limitCheck(&ball);

			// Loses a life if ball goes off the bottom
			if (dead) {
				state = GAMESETUP;
				lives--;
				break;
			}

			// Check collisions with paddle
			paddleCheck(&ball, &paddle);

			// Check collisions with the bricks through the use of intersections
			int width = 0;
			int height = 0;
			for (int i = 0; i < NUMBRICKSROW*NUMBRICKSCOL; i++) {
				BRICK *b = &bricks[i];
				INTERSECTION i = findIntersectionBrick(b, &ball);

				if (i.width > 0 && i.height > 0) {
					width += i.width;
					height += i.height;
				
					if (b->hits == 1) {
						drawRect(b->r, b->c, b->h, b->w, BGCOLOR);
						b->color = BLUE;
						b->r = 239;
						b->c = 159;
						b->w = 1;
						b->h = 1;
						score += 10;
						bricksHit++;
					}
					(b->hits)--;
				}
			}

			// Adjusts ball movement accordingly based on collisions
			collisionCheck(width, height, &ball);

			// Checks if level has been beaten and updates
			if (bricksHit == curLevel.numBricks) {
				level++;
				newLevel = 1;
				lives++;
				switch(level) {
					case 1:
						curLevel = l1;
						break;
					case 2:
						curLevel = l2;
						break;
					case 3:
						curLevel = l3;
						break;
					case 4:
						curLevel = l4;
						break;
					case 5:
						curLevel = l5;
						break;
					default:
						break;
				}
				state = LEVELSTATE;
				break;
			}

			// Redraw ball
			drawRect(ball.oldR, ball.oldC, ball.size, ball.size, BGCOLOR);
			drawImage3(ball.r, ball.c, BALL_HEIGHT, BALL_WIDTH, ball_data);

			// Redraw paddle
			drawRect(155, paddle.oldC, PADDLEHEIGHT, PADDLEWIDTH, BGCOLOR);
			drawRect(155, paddle.c, PADDLEHEIGHT, PADDLEWIDTH, BLUE);
			drawRect(156, paddle.c + 1, PADDLEHEIGHT - 2, PADDLEWIDTH - 2, DARKBLUE);
			
			// Redraw bricks
			for (int i = 0; i < NUMBRICKSROW*NUMBRICKSCOL; i++) {
				BRICK *a = &bricks[i];
				unsigned short color = TEAL;
				unsigned short border = TBOR;
				if (a->hits == 2) {
					color = DARKGREEN;
					border = DGBOR;
				} else if (a->hits == 3) {
					color = DARKPURPLE;
					border = PBOR;
				}
				drawRect(a->r, a->c, a->h, a->w, border);
				drawRect(a->r + 1, a->c + 1, a->h - 2, a->w - 2, color);
			}
			
			// Update old positions
			ball.oldR = ball.r;
			ball.oldC = ball.c;
			paddle.oldC = paddle.c;

			break;

		case LEVELSTATE:

			//Checks if player won
			if (level == 6) {
				state = WIN;
				break;
			}

			// Draw level background and text
			drawImage3(0,0,LEVELSCREEN_WIDTH, LEVELSCREEN_HEIGHT, levelScreen_data);

			drawString(70, 103, "LEVEL", ORANGE);

			sprintf(levelStrVal, "%d", level);
			drawString(80, 115, levelStrVal, ORANGE);

			state = LEVEL_NODRAW;
			while(KEY_DOWN_NOW(BUTTON_START) || KEY_DOWN_NOW(BUTTON_A));

			break;
		case LEVEL_NODRAW:

			// Select returns to title
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = TITLE;
			}

			// A or START move to the next screen
			if(KEY_DOWN_NOW(BUTTON_A)) {
				state = GAMESETUP;
			}

			if(KEY_DOWN_NOW(BUTTON_START)) {
				state = GAMESETUP;
			}

			break;
		case WIN:

			// Draw background and text
			drawImage3(0,0,ENDSCREEN_WIDTH, ENDSCREEN_HEIGHT, endScreen_data);

			drawString(40, 70, "CONGRATULATIONS!", ORANGE);
			drawString(60, 95,  "YOU WON!", ORANGE);
			drawString(100, 40, "PRESS START TO PLAY AGAIN!", ORANGE);

			state = WIN_NODRAW;
			break;

		case WIN_NODRAW:

			// Select returns to title
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = TITLE;
			}

			// A or START also return to title
			if(KEY_DOWN_NOW(BUTTON_A)) {
				state = TITLE;
			}

			if (KEY_DOWN_NOW(BUTTON_START)) {
				state = TITLE;
			}
			
			break;

		case LOSE:

			// Draws background and text
			drawImage3(0,0,ENDSCREEN_WIDTH, ENDSCREEN_HEIGHT, endScreen_data);

			drawString(50, 87, "GAME OVER!", ORANGE);
			drawString(80, 50, "PRESS START TO GIVE IT", ORANGE);
			drawString(90, 78, "ANOTHER SHOT!", ORANGE);

			state = LOSE_NODRAW;
			break;

		case LOSE_NODRAW:

			// Select, Start, and A all return to title
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = TITLE;
			}

			if(KEY_DOWN_NOW(BUTTON_A)) {
				state = TITLE;
			}

			if(KEY_DOWN_NOW(BUTTON_START)) {
				state = TITLE;
			}

			break;
		}
	}
}


