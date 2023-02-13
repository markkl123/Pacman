#include "Pacman.h"


bool Pacman::operator==(const Point& a) const
{
	return GetX() == a.GetX() && GetY() == a.GetY();
}

/// <summary>
/// Moves Pacman on the map properly
/// </summary>
/// <param name="map:">2x matrix map of chars</param>
/// <param name="Key:">Given key for the pacman to go</param>
/// <param name="lastmove:">Last pacman move</param>
void Pacman::Move(char map[25][80], int Xsize, int Ysize, const Keys& Key)
{
	draw(' ');
	move(Key);

	if (map[y][x] == WALL) {
		move(OpKey(Key));
		if (Key != lastmove)
			Move(map, Xsize, Ysize, lastmove);
	}
	else
		lastmove = Key;

	if (y < 0)
		y = Ysize - 1;

	if (y > Ysize - 1)
		y = 0;

	if (x > Xsize - 1)
		x = 0;

	if (x < 0)
		x = Xsize - 1;

	drawPac();
}

void Pacman::SetColor(bool WithColor)
{
	this->WithColor = WithColor;
}

void Pacman::SetPacmanInit(int init[2])
{
	pacman_init[0] = init[0];
	pacman_init[1] = init[1];
	x = pacman_init[0];
	y = pacman_init[1];
}

Pacman::Pacman() : Pacman(0) {};

//Constractor
//Input: Colors: with(True) without(False)
Pacman::Pacman(bool WithColor)
{
	this->WithColor = WithColor;
}

void Pacman::_init_location()
{
	draw(' ');
	x = pacman_init[0];
	y = pacman_init[1];
	drawPac();
}

int Pacman::Get_Lives() const
{
	return lives;
}


int Pacman::Get_GamePoints() const
{
	return pointGame;
}

//Draws pacman
void Pacman::drawPac() const {
	if (WithColor)
		setTextColor(Color::YELLOW);
	draw(PACMAN); 
	if (WithColor)
	setTextColor(Color::RED);
}

//Pacman lost a life 
void Pacman::updateLive() { lives--;}

//Pacman ate a breadcamber
void Pacman::updatePoints(int gp)
{
	pointGame += gp;
}