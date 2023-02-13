#include "Frame.h"

Frame::Frame(Keys pacman, Keys fruit, Keys ghosts[4]) : pacman(pacman), fruit(fruit)
{
	for (int i = 0; i < 4; i++)
		this->ghosts[i] = ghosts[i];
};

int Frame::getFruitX() { return Fruit_X; }
int Frame::getFruitY(){ return Fruit_Y; }
void Frame::setFruitXY(int x, int y)
{
	Fruit_X = x;
	Fruit_Y = y;
}

void Frame::setGhostByIndex(Keys key, int index)
{
	ghosts[index] = key;
}

char* Frame::getGhosts()
{
	char *res = new char[4];
	for (int i = 0; i < 4; i++)
		res[i] = (char)ghosts[i];
	return res;
}

Frame::Frame()
{
	pacman = Keys::STOP;
	fruit = Keys::STOP;
	for (int i = 0; i < 4; i++)
		this->ghosts[i] = Keys::STOP;
}

void Frame::setGhosts(Keys ghosts[4])
{
	for (int i = 0; i < 4; i++)
		this->ghosts[i] = ghosts[i];
}
