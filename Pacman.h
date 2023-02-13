#pragma once
#include "Point.h"

class Pacman : public Point{
private:

	int pacman_init[2];

	int lives = 3, pointGame = 0;

	Keys lastmove = Keys::STOP; /*Last Pacman move*/

	//Colors: with(True) without(False)
	bool WithColor = 0;
public:

	Pacman();
	Pacman(bool WithColor);
	void SetPacmanInit(int init[2]);
	void SetColor(bool WithColor);
	bool operator==(const Point& a) const;
	void Move(char map[25][80], int Xsize, int Ysize ,const Keys& Key);
	void drawPac() const;
	void updateLive();//update the life of the pacman
	void updatePoints(int gp = 1);
	int Get_Lives() const;
	int Get_GamePoints()const;
	void _init_location();
};