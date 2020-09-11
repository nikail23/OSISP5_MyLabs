#include "Sprite.h"
#include <cmath>

Sprite::Sprite(int x, int y, int width, int height, float speed, HANDLE hndSprite)
{
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->speed = speed;
	angle = 0;
	GetObject(hndSprite, sizeof(BITMAP), &Bmp);
}

void Sprite::moveRight(float time)
{
	x += speed * time;
}

void Sprite::moveLeft(float time)
{
	x -= speed * time;
}

void Sprite::moveDown(float time)
{
	y += speed * time;
}

void Sprite::moveUp(float time)
{
	y -= speed * time;
}

int Sprite::getX()
{
	return x;
}

int Sprite::getY()
{
	return y;
}

int Sprite::getWidth()
{
	return width;
}

int Sprite::getHeight()
{
	return height;
}

void Sprite::draw(HWND hWnd, HANDLE hndSprite)
{	
	HDC hdc = GetDC(hWnd);

	SetGraphicsMode(hdc, GM_ADVANCED);

	xform.eM11 = cos(angle);
	xform.eM12 = sin(angle);
	xform.eM21 = -sin(angle);
	xform.eM22 = cos(angle);
	xform.eDx = x;
	xform.eDy = y;
	SetWorldTransform(hdc, &xform);
	
	RECT rect;
	GetClientRect(hWnd, &rect);
	LPtoDP(hdc, (LPPOINT)&rect, 2);
	
	showBitmap(hdc, hndSprite);
	ReleaseDC(hWnd, hdc);
}

void Sprite::showBitmap(HDC winDC, HANDLE hndSprite)
{
	HDC memDC = CreateCompatibleDC(winDC);
    SelectObject(memDC, hndSprite);
	StretchBlt(winDC, -width / 2, -height / 2, width, height, memDC, 0, 0, Bmp.bmWidth, Bmp.bmHeight, SRCCOPY); 
    DeleteDC(memDC);
}

void Sprite::rotateRight(float deltaAngle)
{
	angle += deltaAngle;
}

void Sprite::rotateLeft(float deltaAngle)
{
	angle -= deltaAngle;
}

