#pragma once
#include <iostream>
#include "Pacman.h"
#include "Ghost.h"
#include "Fruit.h"
#include "Frame.h"


class Map
{
	int time = 0;
	int Xsize = 0, Ysize = 0;
	Pacman Pac;
	char map[25][80];
	Ghost Ghosts[4];
	int GhostNum;
	Fruit fruit;
	int BreadCrumbs;
	int Init_Score[2];
	vector<Frame> Record;
	int looseRec[3] = { 0 };
	//Colors: with(True) without(False)
	bool WithColor = 0;

public:

	Map(int Xmapsize, int Ymapsize, char initmap[25][80], int pacinit[2], int Ghostsnum, int ghost_init[4][2], int BREADCRUMBS_AMOUNT, int init_Score[2], WISDOM dificulty, bool WithColor, int lives, int pointGame);
	char get_Point_Visual(int x, int y) const;
	~Map();
	void Move(Keys& Key);
	void print() const;
	void Reset_Map(int Xmapsize, int Ymapsize, char initmap[25][80]);
	int Get_Pacman_gamepoints() const;
	int Get_Pacman_lives() const;
	void MoveGhosts();
	bool CheckForGhost(Keys& Key);
	int GetBreadCrumbs() const;
	void CheckForFood();
	bool IsValidKey(char Key) const;
	void Pause() const;
	void PrintGhosts() const;
	void PrintFruit() const;
	void Draw_Lives() const;
	void Draw_Points() const;
	vector<Frame>& getRecord();
	void SetRecord(vector<Frame> R);
	void UpTime();
	int Get_time();
	int* Get_looseRec();
	void MovebyFrameinithelper();
	void MovebyFrame(Frame& curr, int loose[3], bool silent);
	void MoveGhostsByFrame(Frame& cuur);
};