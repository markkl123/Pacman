#pragma once
#include "Point.h"
class Fruit : public Point
{
	int CountMoves = 0;
	int Value = 0;
	bool Visible = false;
	Keys MoveRand(char map[25][80], int Xsize, int Ysize);
public:
	void SetValue(int value);
	void SetVisible(bool state);
	int GetValue() const;
	void SetXY(int x, int y);
	bool IsVisible() const;
	void drawFruit() const;
	bool Move(char map[25][80], int Xsize, int Ysize,const Keys& key);
	Keys MoveFruit(char map[25][80], int Xsize, int Ysize);
};

