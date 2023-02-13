#include "Fruit.h"

//Move random one step
Keys Fruit::MoveRand(char map[25][80],int Xsize,int Ysize) {
	Keys key;
	bool DoAgain = true;
	do {
			int dir = rand() % 4;
			switch (dir)
			{
			case 0:
				key = Keys::UP;
				break;
			case 1:
				key = Keys::DOWN;
				break;
			case 2:
				key = Keys::LEFT;
				break;
			case 3:
				key = Keys::RIGHT;
				break;
			}
	} while (!Move(map, Xsize, Ysize, key));
	return key;
}

//Return true if move suceed
bool Fruit::Move(char map[25][80], int Xsize, int Ysize,const Keys& key) {
	move(key);

	if (map[GetY()][GetX()] == WALL) {
		move(OpKey(key));
		return false;
	}
	if (y < 0 || y > Ysize - 1 || x < 0 || x > Xsize - 1) {
		move(OpKey(key));
		move(OpKey(key));
	}
	return true;
}

void Fruit::SetVisible(bool state)
{
	Visible = state;
}

bool Fruit::IsVisible() const
{
	return Visible;
}

int Fruit::GetValue() const
{
	return Value;
}

void Fruit::drawFruit() const
{
	if (Visible)
	draw('0'+ Value);
}

void Fruit::SetXY(int x, int y)
{
	this->x = x;
	this->y = y;
}

void Fruit::SetValue(int value)
{
	Value = value;
}

//Move fruit if visible, if not then randomly turn on may occure
Keys Fruit::MoveFruit(char map[25][80], int Xsize, int Ysize )
{
	Keys res = Keys::STOP;
	if (Visible)
	{
		draw(map[y][x]);
		if (CountMoves == 50)
			Visible = false;
		else if (CountMoves % 5 == 2)
		{
			res = MoveRand(map, Xsize, Ysize);
			CountMoves++;
		}
		else
			CountMoves++;
	}
	else
	{
		int dir = rand() % 70;
		if (dir == 8)
		{
			int a, b;
			do {
				a = rand() % Xsize;
				b = rand() % Ysize;
			} while (map[b][a] == WALL || map[b][a] == UNVISITED);

			x = a;
			y = b;
			Visible = true;
			CountMoves = 0;
			dir = rand() % 5;
			Value = dir + 5;
		}
	}
	if (Visible)
		drawFruit();
	return res;
}