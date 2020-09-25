#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>

enum Direction { UP, DOWN, LEFT, RIGHT };

class Sprite
{
	float speed, scale;
	int x, y, width, height;
	BITMAP Bmp;
	RECT window;

	void checkBorders();
public:
	Sprite(int x, int y, int width, int height, float speed, HANDLE hndSprite, RECT window);
	void move(Direction direction);
	void draw(HDC winDC, HANDLE hndSprite);
	void setScale(float deltaScale);
	void setWindowRect(RECT window);
};

#endif
