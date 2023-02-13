// Pacman.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Menu.h"

using namespace std;

int main(int argc, char** argv)
{
	clear_screen();
	PlayMode mode = PlayMode::Regular;
	try {
		if (argc > 1)
			mode = GamePlay::strtomode(argv, argc);

		if (mode == PlayMode::Load || mode == PlayMode::Silent)
			GamePlay::PlayByMaps(false, mode);
		else
			while (true)
			{
				switch (Menu::menu())
				{
				case Menu::COLORGAME:
					Menu::ChooseGameForm(true, mode);
					break;
				case Menu::UNCOLORGAME:
					Menu::ChooseGameForm(false, mode);
					break;
				case Menu::INSTRUCTIONS:
					Menu::Print_Instruction();
					break;
				case Menu::EXIT:
					return 1;
					break;
				}
			}
	}
	catch (const char* e)
	{
		cout.clear();
		cout << e;
	}
}