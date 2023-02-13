#pragma once
#include "Point.h"


enum class WISDOM
{
	BEST = 2,
	GOOD = 1,
	NOVICE = 0
};

class Ghost : public Point {
private:
	int CountMoves = 0;
	Point ghost_init;
	WISDOM Mode;
	static list<Point> Forbidenmem;
	Keys forbiden = Keys::STOP; /*Forbiden ghost move*/
	Keys MoveNovice(char map[25][80], int Xsize, int Ysize);
	Keys MoveGood(char map[25][80], int Xsize, int Ysize, const Point& pac);
	Keys MoveRand(char map[25][80], int Xsize, int Ysize);
	Keys MoveSmart(char map[25][80], int Xsize, int Ysize, const Point& pac);
	Keys* GenerateSmartMove(const Point& pac);
	int GHOST_INIT[2];
public:
	void _Set_GHOST_INIT(int parr[2]);
	void SetGhostInit(const Point& p);
	bool Move(char map[25][80], int Xsize, int Ysize, Keys& key);
	Keys MoveGhost(char map[25][80], int Xsize, int Ysize,const Point& pac);
	Ghost();
	Ghost(const Point& p, const WISDOM &m );
	Ghost& operator=(const Point& p);
	void DrawGhost() const;
	void SetWisdom(const WISDOM& m);
	void _init_location();
};

