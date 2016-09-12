
#define REG_DISPCTL *(unsigned short *)0x4000000
#define BG2_ENABLE (1<<10)
#define MODE3 3
#define SCANLINECOUNTER *(volatile unsigned short *)0x4000006

#define COLOR(r, g, b)   ((r) | (g)<<5 | (b)<<10)
#define COlORRGB(r, g, b) (((r) / 8) | ((g) / 8) <<5 | ((b) / 8)<<10)

#define RED COLOR(31,0,0)
#define GREEN COLOR(0, 16+15, 0)
#define BLUE COLOR(0,0,31)
#define MAGENTA COLOR(31, 0, 31)
#define CYAN COLOR(0, 10, 10)
#define YELLOW COLOR(31, 31, 0)
#define WHITE COLOR(31,31,31)
#define ORANGE COLOR(30,15,0)
#define GRAY COLOR(2,2,2)
#define DARKBLUE COlORRGB(0,0,102)
#define DARKGREEN COlORRGB(0, 102, 0)
#define DGBOR COlORRGB(0, 153, 0)
#define DARKPURPLE COlORRGB(102, 0, 102)
#define PBOR COlORRGB(153, 0, 153)
#define DARKORANGE COlORRGB(255, 102, 0)
#define TEAL COlORRGB(0, 102, 102)
#define TBOR COlORRGB(0, 153, 153)
#define BLACK 0

#define OFFSET(r, c, rowlen) ((r)*(rowlen) + (c))

// Buttons

#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

#define KEY_DOWN_NOW(key)  (~(BUTTONS) & key)

#define BUTTONS *(volatile unsigned int *)0x4000130


extern unsigned short *videoBuffer;

/* DMA */

#define REG_DMA0SAD         *(const volatile u32*)0x40000B0 // source address
#define REG_DMA0DAD         *(volatile u32*)0x40000B4       // destination address
#define REG_DMA0CNT         *(volatile u32*)0x40000B8       // control register

// DMA channel 1 register definitions
#define REG_DMA1SAD         *(const volatile u32*)0x40000BC // source address
#define REG_DMA1DAD         *(volatile u32*)0x40000C0       // destination address
#define REG_DMA1CNT         *(volatile u32*)0x40000C4       // control register

// DMA channel 2 register definitions
#define REG_DMA2SAD         *(const volatile u32*)0x40000C8 // source address
#define REG_DMA2DAD         *(volatile u32*)0x40000CC       // destination address
#define REG_DMA2CNT         *(volatile u32*)0x40000D0       // control register

// DMA channel 3 register definitions
#define REG_DMA3SAD         *(const volatile u32*)0x40000D4 // source address
#define REG_DMA3DAD         *(volatile u32*)0x40000D8       // destination address
#define REG_DMA3CNT         *(volatile u32*)0x40000DC       // control register

typedef struct
{
	const volatile void *src;
	const volatile void *dst;
	unsigned int cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)


// Game Values
#define BW 15
#define BH 5
#define NUMBRICKSROW 5
#define NUMBRICKSCOL 10
#define BALLSIZE 5
#define BALLCOLOR BLUE
#define TOLL 1
#define STARTINGLOCR 150
#define STARTINGLOCC 123
#define STARTINGLIVES 3
#define BGCOLOR BLACK
#define PADDLEHEIGHT 4
#define PADDLEWIDTH 30


// Brick that are to be broken
typedef struct {
	int r; 
	int c;
	int h;
	int w;
	unsigned short color;
	int hits;
} BRICK;

// The ball for which to break the bricks
typedef struct {
	int r;
	int c;
	int oldR;
	int oldC;
	int size;
	int color;
	int rMov;
	int cMov;
} BALL;

// The paddle to hit the ball to break the bricks
typedef struct {
	int c;
	int cMov;
	int oldC;
	unsigned short color;
} PADDLE;

// An intersection between balls, bricks, and paddles
typedef struct {
	int width;
	int height;
} INTERSECTION;

// A level to play and break the bricks with the paddle and the ball to win the game and celebrate and have fun and jump for joy and go and tell the world of your accomplishments
typedef struct {
	int levelNum;
	int *levelBricks;
	int numBricks;
} LEVEL;

// States for game's state machine
enum GBAState {
	TITLE,
	TITLE_NODRAW,
	GAMESETUP,
	GAME,
	WIN,
	WIN_NODRAW,
	LOSE,
	LOSE_NODRAW,
	LEVELSTATE,
	LEVEL_NODRAW
};


// Prototypes
// Displays
void waitForVblank();
void setPixel(int row, int col, unsigned short color);
void drawRect(int row, int col, int height, int width, volatile unsigned short color);
void drawImage3(int r, int c, int width, int height, const unsigned short* image);
void fillScreen(volatile unsigned short color);

// Timing
void delay(int n);

// Collisions
int limitCheck(BALL *ball);
void collisionCheck(int width, int height, BALL *ball);
INTERSECTION findIntersectionBrick(BRICK *b, BALL *ball);
void paddleCheck(BALL *ball, PADDLE *p);
INTERSECTION findIntersectionPaddle(BALL *ball, PADDLE *p);

//Game setup
BRICK* brickMaker(int *level, BRICK *bricks);
