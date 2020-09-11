#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <windows.h>

class Sprite
{
	float speed;
	int x, y, width, height;
	float angle;
	BITMAP Bmp;
	XFORM xform;
	void showBitmap(HDC winDC, HANDLE hndSprite);

public:
	Sprite(int x, int y, int width, int height, float speed, HANDLE hndSprite);
	void moveRight(float time);
	void moveLeft(float time);
	void moveUp(float time);
	void moveDown(float time);
	void rotateRight(float deltaAngle);
	void rotateLeft(float deltaAngle);
	int getX();
	int getY();
	int getWidth();
	int getHeight();
	void draw(HWND hWnd, HANDLE hndSprite);
};

#endif
