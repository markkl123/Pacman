#include "Ghost.h"

list<Point> Ghost::Forbidenmem = list<Point>();

Ghost::Ghost(const Point &p,const WISDOM &m) : Point(p.GetX(),p.GetY())
{
	Forbidenmem.clear();
	Mode = m;
	ghost_init = p;
	_init_location();
}

Ghost::Ghost() : Ghost((Point)Point(),WISDOM::BEST) {};

void Ghost::SetGhostInit(const Point& p)
{
	this->ghost_init = p;
}
Ghost& Ghost::operator=(const Point& p)
{
	x = p.GetX();
	y = p.GetY();
	return *this;
}

void Ghost::DrawGhost() const
{
	draw(GHOST);
}

void Ghost::SetWisdom(const WISDOM& m)
{
	Mode = m;
}

void Ghost::_init_location()
{
	*this = ghost_init;
}

void Ghost::_Set_GHOST_INIT(int parr[2])
{
		GHOST_INIT[0] = parr[0];
		GHOST_INIT[1] = parr[1];
}

//Returns Array of Keys[4] for best move to reach the pacman
Keys* Ghost::GenerateSmartMove(const Point& pac)
{
	Keys *res = new Keys[4];
	int xdif, ydif;
	xdif = pac.GetX() - x;
	ydif = pac.GetY() - y;
	if (abs(xdif) > abs(ydif))
	{
		res[0] = xdif > 0 ? Keys::RIGHT : Keys::LEFT;
		res[1] = ydif > 0 ? Keys::DOWN : Keys::UP;
		res[2] = ydif <= 0 ? Keys::DOWN : Keys::UP;
		res[3] = xdif <= 0 ? Keys::RIGHT : Keys::LEFT;
	}
	else
	{
		res[0] = ydif > 0 ? Keys::DOWN : Keys::UP;
		res[1] = xdif > 0 ? Keys::RIGHT : Keys::LEFT;
		res[2] = xdif <= 0 ? Keys::RIGHT : Keys::LEFT;
		res[3] = ydif <= 0 ? Keys::DOWN : Keys::UP;
	}

	for (int i = 0; i < 3; i++)
		if (res[i] == forbiden)
		{
			res[i] = res[i + 1];
			res[i + 1] = forbiden;
		}

		
	return res;
}

Keys Ghost::MoveGhost(char map[25][80], int Xsize, int Ysize, const Point& pac)
{
	switch (Mode)
	{
	case WISDOM::BEST:
		return MoveSmart(map, Xsize, Ysize, pac);	
	case WISDOM::GOOD:
		return MoveGood(map, Xsize, Ysize, pac);
	case WISDOM::NOVICE:
		return MoveNovice(map, Xsize, Ysize);
	}
	return Keys::STOP;
}

Keys Ghost::MoveGood(char map[25][80], int Xsize, int Ysize, const Point& pac)
{
	Keys res;
	int dir = rand() % 20;
	
	if (dir == 0 || CountMoves != 0)
	{
		CountMoves++;
		if (CountMoves == 5)
			CountMoves = 0;
		res = MoveRand(map, Xsize, Ysize);
	}
	else
		res = MoveSmart(map, Xsize, Ysize, pac);
	return res;
}

Keys Ghost::MoveNovice(char map[25][80], int Xsize, int Ysize)
{
	Keys res;
	CountMoves++;
	if (forbiden == Keys::STOP || CountMoves == 20)
	{
		if (CountMoves == 20)
			CountMoves = 0;
		res = MoveRand(map, Xsize, Ysize);
	}
	else
	{
		Keys key = OpKey(forbiden);
		Move(map, Xsize, Ysize, key);
		res = key;
	}
	return res;
}

//Move one step random
Keys Ghost::MoveRand(char map[25][80], int Xsize, int Ysize) {
	int i = 0;
	Keys key = Keys::STOP;
	bool DoAgain = true;
	do {
		i++;
		do {
			if (i == 8)
			{
				key = forbiden;
				break;
			}
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
		} while (key == forbiden);
	} while (!Move(map, Xsize, Ysize, key));
	return key;
}

//Ghosts share memory
Keys Ghost::MoveSmart(char map[25][80], int Xsize, int Ysize,const Point& pac) {
	Keys memKeyhelper;
	Point memPointhelper = (Point)*this;
	bool ispacforbiden = find(Forbidenmem.begin(), Forbidenmem.end(), pac) != Forbidenmem.end();
	bool currforbiden = find(Forbidenmem.begin(), Forbidenmem.end(), *this) != Forbidenmem.end();
	int i;
	Keys* Moves = GenerateSmartMove(pac);
	for (i = 0; i < 4; i++)
	{
		if (i == 3)
			Forbidenmem.push_back(*this);
		else
			memKeyhelper = Moves[i];
		if (Move(map, Xsize, Ysize, Moves[i]))
		{
			if (memKeyhelper != Moves[i])
				Forbidenmem.push_back(memPointhelper);
			
			if (!currforbiden && !ispacforbiden && find(Forbidenmem.begin(), Forbidenmem.end(), *this) != Forbidenmem.end())
			{
				Moves[i] = OpKey(Moves[i]);
				Move(map,Xsize,Ysize, Moves[i]);
			}
			else
				break;
		}
	}
	Keys res = (i == 4 ? Moves[3] : Moves[i]);
	delete[] Moves;
	return res;
}

//Returns true if move succeed
bool Ghost::Move(char map[25][80], int Xsize, int Ysize, Keys& key) {
		move(key);

		if (map[GetY()][GetX()] == WALL) {
			move(OpKey(key));
			return false;
		}
		if (y < 0 || y > Ysize - 1 || x < 0 || x > Xsize - 1) {
			move(OpKey(key));
			move(OpKey(key));
			key = OpKey(key);
		}
		forbiden = OpKey(key);
		return true;
}