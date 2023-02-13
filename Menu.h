#pragma once
#include "GamePlay.h"
using namespace std;

class Menu
{
public:
	enum GameOptions {
		COLORGAME = 1,
		UNCOLORGAME = 2,
		INSTRUCTIONS = 8,
		EXIT = 9
	};

	static void ChooseGameForm(bool withcolor, PlayMode mode);
	static void Print_Instruction();
	static int menu();
	static bool IsInGameOptions(int num);
};

