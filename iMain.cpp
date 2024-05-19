#include "iGraphics.h"
#include<iostream>
#include<cstdlib>
#include<ctime>

using namespace std;

#define screenWidth 1280
#define screenHeight 720
#define backGroundSpeed 5
#define ENEMY_NUMBER 14
#define BULLET_SPEED 15
#define MAX_BULLETS 50

int score = 0;
int totalScore = 0;
char showScore[10000000];
char showtotalScore[10000000];
int bgimage1 = 0;
int bgimage2 = -720;
int homePage = 0;
int instructPage = 0;
int playPage = 0;
int highScorePage = 0;
int aboutPage = 0;
int enter = 0;
int musicOn = 1;
int play = 0;
int enemySpacing = 100;
int life = 10;
int lifeImageWidth = 30;
int lifeImageHeight = 30;
int lifespacing = 5;
int column;
int gameOverImage;
bool spaceKeyPressed = false;
bool gameOver = false;

char loadImage[6][25] = { "img\\Menu\\pre\\1.bmp", "img\\Menu\\pre\\2.bmp", "img\\Menu\\pre\\3.bmp", "img\\Menu\\pre\\4.bmp", "img\\Menu\\pre\\5.bmp", "img\\Menu\\logo.bmp" };
int loadIndex = 0;
int loadingscreen = 1;

void functionLoad();
void drawHomePage();
//void drawInstructPage();
void drawScore();
void drawPlayPage();
void initializeEnemy();
void drawEnemies();
void moveEnemies();
void drawBullets();
void moveBullets();
void drawHighScorePage();
void drawAboutPage();
void drawInstractionPage();
void playButton();
void hsButton();
void aboutButton();
void backButton();
//void instructButton();
struct Enemy {
	int x;
	int y;
	bool active;
};
struct Bullet {
	int x;
	int y;
	bool active;
};
Bullet bullets[MAX_BULLETS];
Enemy enemy[ENEMY_NUMBER];


int a;
int x = 0;
int y = 0;
int sx = 0;
int dy = 0;

void iDraw()
{
	iClear();
	if (loadingscreen == 1){
		iSetColor(0, 0, 0);
		iFilledRectangle(0, 0, 1280, 720);
		iShowBMP2(0, 0, loadImage[loadIndex], 0);
	}
	else if (homePage == 1){
		drawHomePage();
	}
	else if (playPage == 1){
		if (life <= 0)
			gameOver = true;
		else{
			drawPlayPage();
			drawScore();
		}
	}

	else if (highScorePage == 1){
		drawHighScorePage();
	}

	else if (aboutPage == 1){
		drawAboutPage();
	}

	else if (instructPage == 1){
		drawInstractionPage();
	}

	if (gameOver){
		totalScore = score;
		score = 0;
		playPage = 0;
		iShowBMP(0, 0, "img\\gameOver.bmp");
		sprintf_s(showtotalScore, "%d", totalScore);
		iText(665, 297, showScore, GLUT_BITMAP_TIMES_ROMAN_24);
	}

}



void drawHomePage()
{
	iSetColor(128, 128, 128);
	iFilledRectangle(0, 0, 1280, 720);
	iShowBMP2(0, 0, "img\\Menu\\Menubg.bmp", 0);
}

void drawAboutPage(){
	iFilledRectangle(0, 0, 1280, 720);
	iShowBMP2(0, 0, "img\\Menu\\aboutUs.bmp", 0);
}

void drawHighScorePage(){
	iFilledRectangle(0, 0, 1280, 720);
	iShowBMP2(0, 0, "img\\Menu\\highScore.bmp", 0);
}

void drawInstractionPage(){
	iFilledRectangle(0, 0, 1280, 720);
	iShowBMP2(0, 0, "img\\Menu\\instruction.bmp", 0);
}

void drawScore(){
	if (playPage == 1){

		iSetColor(255, 255, 255);
		sprintf_s(showScore, "%d", score);
		iText(50, 680, "Score:", GLUT_BITMAP_TIMES_ROMAN_24);
		iText(120, 680, showScore, GLUT_BITMAP_TIMES_ROMAN_24);
	}
}
void initializeEnemies() {
	for (int i = 0; i < ENEMY_NUMBER; i++) {
		enemy[i].x = rand() % screenWidth;
		enemy[i].y = screenHeight + rand() % 300;
		enemy[i].active = true;
	}
}
int startX = 0;

void drawPlayPage(){

	iShowImage(0, bgimage1, 1280, 720, a);
	iShowImage(0, bgimage2, 1280, 720, a);
	bgimage1 += 7;
	bgimage2 += 7;
	if (bgimage1 >= screenHeight)
	{
		bgimage1 = -screenHeight;
	}
	if (bgimage2 >= screenHeight)
	{
		bgimage2 = -screenHeight;
	}
	iShowBMP2(sx, 15, "img\\Menu\\spaceship.bmp", 0);
	int speed = 1; // You can adjust the speed according to your needs
	for (int i = 0; i < ENEMY_NUMBER; i++) {
		if (enemy[i].active) {
			enemy[i].y -= speed; // Move enemy up

			if (enemy[i].y < -80) {
				// If enemy goes off-screen, reset to the bottom
				int nextRow = rand() % 14;
				enemy[i].y = screenHeight + (nextRow * 80);
				enemy[i].x = startX % 14 + 7;
			}

			// Draw the enemy image at the updated position
			iShowBMP2(enemy[i].x, enemy[i].y - dy, "img\\enemy2.bmp", 0);
		}
	}
	for (int i = 0; i < life; i++) {

		int lifeImageX = screenWidth - (lifeImageWidth + lifespacing) * (i + 1);
		int lifeImageY = screenHeight - lifeImageHeight - 5;
		iShowImage(lifeImageX, lifeImageY, lifeImageWidth, lifeImageHeight, iLoadImage("img\\life.bmp"));
	}
	for (int i = 0; i < ENEMY_NUMBER; i++) {
		if (enemy[i].active && ((enemy[i].x - sx <= 200 && enemy[i].x >= sx) || (sx - enemy[i].x <= 100 && sx>enemy[i].x)) && enemy[i].y <= 155) {
			enemy[i].active = false;
			life--;
			if (life <= 0){
				gameOver = true;
			}
		}
	}

	drawEnemies();
	moveEnemies();
	drawBullets();
	moveBullets();
}

void drawEnemies() {
	for (int i = 0; i < ENEMY_NUMBER; i++) {
		if (enemy[i].active) {
			iShowBMP2(enemy[i].x, enemy[i].y - dy, "img\\enemy2.bmp", 0);
		}
	}
}



void moveEnemies() {
	int speed = 5;
	for (int i = 0; i < ENEMY_NUMBER; i++) {
		if (enemy[i].active) {
			enemy[i].y -= speed;
			if (enemy[i].y < -80) {
				int nextRow = rand() % 14;
				enemy[i].y = screenHeight + (nextRow * 80);
				enemy[i].x = rand() % screenWidth;
			}
		}
	}
}




void drawBullets() {

	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].active) {
			iShowBMP2(bullets[i].x, bullets[i].y, "img\\bullet.bmp", 0);
		}
	}

}

void moveBullets() {
	for (int i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].active) {
			bullets[i].y += BULLET_SPEED;

			// Check for collision with enemies
			for (int j = 0; j < ENEMY_NUMBER; j++) {
				if (enemy[j].active &&
					bullets[i].x >= enemy[j].x &&
					bullets[i].x <= enemy[j].x + 80 &&
					bullets[i].y >= enemy[j].y - dy &&
					bullets[i].y <= enemy[j].y + 80 - dy) {
					// Handle collision (you can increase the score or destroy the enemy)
					enemy[j].active = false;
					bullets[i].active = false;
					// Increment the score (you may need to modify this based on your scoring system)
					score += 10;
				}
			}

			// Deactivate bullets that go off-screen
			if (bullets[i].y > screenHeight) {
				bullets[i].active = false;
			}
		}
	}
}


void playButton(){
	homePage = 0;
	instructPage = 0;
	playPage = 1;
	highScorePage = 0;
	aboutPage = 0;
	initializeEnemies();
}

void hsButton(){
	homePage = 0;
	instructPage = 0;
	playPage = 0;
	highScorePage = 1;
	aboutPage = 0;
}

void aboutButton(){
	homePage = 0;
	instructPage = 0;
	playPage = 0;
	highScorePage = 0;
	aboutPage = 1;
}

void instructionButton(){
	homePage = 0;
	instructPage = 1;
	playPage = 0;
	highScorePage = 0;
	aboutPage = 0;
}

void backButton(){
	homePage = 1;
	instructPage = 0;
	playPage = 0;
	highScorePage = 0;
	aboutPage = 0;
}

void functionLoad(){
	if (loadIndex < 5){
		loadIndex++;
	}
	else if (loadIndex == 5){
		enter = 1;
	}
}


/*function iMouseMove() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/


void iMouseMove(int mx, int my)
{
}
//*******************************************************************ipassiveMouse***********************************************************************//
void iPassiveMouseMove(int mx, int my)
{

}

void iMouse(int button, int state, int mx, int my)
{
	printf("x = %d     y = %d\n", mx, my);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (enter == 1 && loadingscreen == 1){
			loadingscreen = 0;
			homePage = 1;
		}

		else if (homePage == 1 && (mx >= 500 && mx <= 700) && (my >= 410 && my <= 475)){
			playButton();
		}

		else if (homePage == 1 && (mx >= 500 && mx <= 700) && (my >= 300 && my <= 370)){
			hsButton();
		}

		else if (homePage == 1 && (mx >= 500 && mx <= 700) && (my >= 185 && my <= 260)){
			aboutButton();
		}

		else if (homePage == 1 && (mx >= 500 && mx <= 700) && (my >= 80 && my <= 150)){
			instructionButton();
		}

		if (!homePage && !playPage && (mx >= 1030 && mx <= 1220) && (my >= 45 && my <= 100)){
			backButton();
		}

		if (gameOver && !playPage && (mx >= 1030 && mx <= 1220) && (my >= 45 && my <= 100)){
			backButton();
			homePage = 1;
			gameOver = false;
			life = 10;
		}
	}



	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{

	}
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/


void iKeyboard(unsigned char key)
{
	if (key == 'f'){
		for (int i = 0; i < MAX_BULLETS; i++)
		{
			if (!bullets[i].active)
			{
				// Set bullet position to be relative to the spaceship
				bullets[i].x = sx + 88; // Center of the spaceship
				bullets[i].y = 50 + dy; // Adjust as needed, and consider spaceship's y-coordinate (dy)
				bullets[i].active = true;
				cout << "pressed" << endl;
				break;
			}
		}
	}
}
/*
function iSpecialKeyboard() is called whenver user hits special keys like-
function keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{


	if (key == GLUT_KEY_RIGHT)
	{
		if (sx < 1280 - 5 - 200)
		{
			sx += 10;
		}
		else
		{
			sx = 1280 - 5 - 200;
		}

		if (!play && playPage){

			play = 1;
		}

	}
	if (key == GLUT_KEY_LEFT)
	{
		if (sx > 5){
			sx -= 10;
		}
		else{
			sx = 5;
		}
		if (!play && playPage){
			play = 1;
		}
	}


	if (key == GLUT_KEY_HOME)
	{

	}

}


int main()
{
	srand((unsigned)time(NULL));
	iSetTimer(500, functionLoad);
	iInitialize(screenWidth, screenHeight, "Cosmic Combat: Alien Strike");
	a = iLoadImage("img\\BG.bmp");
	if (musicOn){
		//PlaySound("Music\\Intro.wav", NULL, SND_LOOP | SND_ASYNC);
	}


	iStart();
	///updated see the documentations
	return 0;
}
