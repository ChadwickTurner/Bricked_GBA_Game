#include "myLib.h"
#include <stdlib.h>

unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color)
{
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, volatile unsigned short color)
{
	for (int i = 0; i < height; i++) {
		DMA[DMA_CHANNEL_3].src = &color;
		DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row + i, col, 240);
		DMA[DMA_CHANNEL_3].cnt = width | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
	}
}

void drawImage3(int r, int c, int width, int height, const unsigned short* image)
{
	for (int i = 0; i < height; i++) {
		DMA[DMA_CHANNEL_3].src = image + width * i;
		DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(r + i, c, 240);
		DMA[DMA_CHANNEL_3].cnt = width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT | DMA_ON;
	}
	
}

void fillScreen(volatile unsigned short color) {
	DMA[DMA_CHANNEL_3].src = &color;
	DMA[DMA_CHANNEL_3].dst = videoBuffer;
	DMA[DMA_CHANNEL_3].cnt = 240*160 | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT | DMA_ON;
}

void delay(int n)
{
	volatile int x=0;
	for(int i=0; i<10000*n; i++)
	{
		x = x + 1;
	}
}


// Makes sure the ball is in bounds and adjusts movement accordingly
int limitCheck(BALL *ball)
{
	int r = ball->r;
	int c = ball->c;
	int size = ball->size;

	if (r + size > 155 + PADDLEHEIGHT){
		return 1;
	}

	if( r < 0) {
		ball->rMov = -ball->rMov;
	}
	if (c + ball->cMov + size > 239-40 || c + ball->cMov < 40) {
		ball->cMov = -ball->cMov;
	}
	return 0;
		

}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}


// Adjusts the balls movement based on collisions with a brick
void collisionCheck(int width, int height, BALL *ball) {
	if (width > 0 && height > 0) {
		if (width >= height) {
			ball->rMov = -ball->rMov;
		} 
		if (width <= height) {
			ball->cMov = -ball->cMov;
		} 
	}

}

// Adjusts the balls movement based on a collision with the paddle
void paddleCheck(BALL *ball, PADDLE *p) {
	INTERSECTION i = findIntersectionPaddle(ball, p);
	if (i.width > 0 && i.height > 0) {
		int paddleMid = (p->c + PADDLEWIDTH - p->c)/2 + p->c;
		int ballMid = ball->c + 2;

		if (ballMid < paddleMid - 10) {
			ball->cMov = -2;
			ball->rMov = -1;	
		} else if (ballMid < paddleMid - 5) {
			ball->cMov = -1;
			ball->rMov = -1;
		} else if (ballMid < paddleMid - 1) {
			ball->cMov = -1;
			ball->rMov = -2;
		} else if (ballMid < paddleMid + 1) {
			ball->cMov = 0;
			ball->rMov = -2;
		} else if (ballMid < paddleMid + 5) {
			ball->cMov = 1;
			ball->rMov = -2;
		} else if (ballMid < paddleMid + 10) {
			ball->cMov = 1;
			ball->rMov = -1;
		} else {
			ball->cMov = 2;
			ball->rMov = -1;
		}
	}
}

// Finds the intersection of the ball and a brick for collision checking
INTERSECTION findIntersectionBrick(BRICK *b, BALL *ball) {
	int br1 = b->r;
	int br2 = b->r + BH;
	int bc1 = b->c;
	int bc2 = b->c + BW;

	int lr1 = ball->r;
	int lr2 = ball->r + BALLSIZE;
	int lc1 = ball->c;
	int lc2 = ball->c + BALLSIZE;

	if (lr1 < br1) {
		lr1 = br1;
	}
	if (lr2 > br2) {
		lr2 = br2;
	}
	if (lc1 < bc1) {
		lc1 = bc1;
	}
	if (lc2 > bc2) {
		lc2 = bc2;
	}

	int width = lc2 - lc1;
	int height = lr2 - lr1;
	INTERSECTION i = {width, height};
	return i;
}

// Finds the intersection between the ball and the paddle for collision checking
INTERSECTION findIntersectionPaddle(BALL *ball, PADDLE *p) {
	int br1 = 155;
	int br2 = 155 + PADDLEHEIGHT;
	int bc1 = p->c;
	int bc2 = p->c + PADDLEWIDTH;

	int lr1 = ball->r;
	int lr2 = ball->r + BALLSIZE;
	int lc1 = ball->c;
	int lc2 = ball->c + BALLSIZE;

	if (lr1 < br1) {
		lr1 = br1;
	}
	if (lr2 > br2) {
		lr2 = br2;
	}
	if (lc1 < bc1) {
		lc1 = bc1;
	}
	if (lc2 > bc2) {
		lc2 = bc2;
	}

	int width = lc2 - lc1;
	int height = lr2 - lr1;
	INTERSECTION i = {width, height};
	return i;
}

// Takes the level array input and assigns the locations to the bricks
BRICK* brickMaker(int *level, BRICK *bricks) {
	int index = 0;
	for (int i = 0; i < 80; i++) {
		if (level[i] > 0) {
			bricks[index].r = 4 + i/10 * (BH + 4);
			bricks[index].c = 40 + i%10 * (BW + 1);
			bricks[index].h = BH;
			bricks[index].w = BW;
			bricks[index].hits = level[i];
			index++;
		}
	}
	return bricks;
}