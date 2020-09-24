#include "Sprite.h"
#include <cmath>

Sprite::Sprite(int x, int y, int width, int height, float speed, HANDLE hndSprite, RECT window)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->speed = speed;
	this->window = window;
	scale = 1;
	GetObject(hndSprite, sizeof(BITMAP), &Bmp);
}

void Sprite::move(Direction direction)
{
	switch (direction)
	{
		case UP:
			y = y - speed;
			break;
		case DOWN:
			y = y + speed;
			break;
		case LEFT:
			x = x - speed;
			break;
		case RIGHT:
			x = x + speed;
			break;
	}
	checkBorders();
}

void Sprite::draw(HDC winDC, HWND hWnd, HANDLE hndSprite)
{
	int halfWidth = width * scale / 2;
	int halfHeight = height * scale / 2;
	HDC memDC = CreateCompatibleDC(winDC);
	SelectObject(memDC, hndSprite);
	StretchBlt(winDC, x - halfWidth, y - halfHeight, width * scale, height * scale, memDC, 0, 0, Bmp.bmWidth, Bmp.bmHeight, SRCCOPY);
	DeleteDC(memDC);
}

void Sprite::setScale(float deltaScale)
{
	scale += deltaScale;
	if (scale < 1)
		scale = 1;
}

void Sprite::checkBorders()
{
	int halfWidth = width * scale / 2;
	int halfHeight = height * scale / 2;

	if (x + halfWidth > window.right)
		x = window.right - halfWidth;
	if (x - halfWidth < 0)
		x = halfWidth;
	if (y + halfHeight > window.bottom)
		y = window.bottom - halfHeight;
	if (y - halfHeight < 0)
		y = halfHeight;
}

void Sprite::setWindowRect(RECT window)
{
	this->window = window;
	checkBorders();
}

