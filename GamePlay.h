#pragma once
#include "map.h"

enum class PlayMode
{
	Save = 0,
	Load = 1,
	Silent = 2,
	Regular = 3
};

class GamePlay
{
private:
	static void Play(Map &map);
	static WISDOM ChooseDificulty();
	static vector<Frame> importMap(string mapPath);
	static void exportMap(string mapName, vector<Frame> steps, int loseTime[3], int loseSize, int finishScreen, int numOfGhost);
	static void createMapFromFile(string line, char board[25][80],int pacPoss[2],int livesPoss[2],int& breadcrumb,int& ySize, int xSize,int Ghost[4][2],int& numOfGhost,bool livesInFirstLine, int& numOfPac);
	static void insertData(string line, char lineCopy[80],int pacPoss[2],int livesPoss[2], int Ghost[4][2],int& numOfGhost,int& breadcrumb, int ySize,int xSize, int& numOfPac);
	static bool checkForLiveInformationInFirstLine(string firstLine,int& xSize,int livesPoss[2]);
	static void validGame(int numOfPac, int numOfGhost,int xSize, int ySize, int pacPoss[2],int Ghost[4][2]);
    static void PlayByRec(Map& board, vector<Frame> Rec, int loose_time[3], bool silent = false);
	static void setFirstFrame(Frame& frame);
	static void createframe(string line,Frame& frame);
	static void returnRes(string mapName,int loose_time[3], int &RecTime, bool &isWin);

public:
	static PlayMode strtomode(char** strs, int size);
	static void PlayByMaps(bool WithColor, PlayMode mode);
	static void PlaySpecificMap(bool WithColor, PlayMode mode);
};

