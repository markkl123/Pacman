#include "Menu.h"

//Print the instructions
void Menu::Print_Instruction()
{
	hideCursor();
	cout << "Keys:" << endl << endl;
	cout << "LEFT: {a or A}" << endl;
	cout << "RIGHT:	{d or D}" << endl;
	cout << "UP: {w or W}" << endl;
	cout << "Down: {x or X}" << endl;
	cout << "STAY: {s or S}" << endl;
	cout << "Pressing the ESC key during a game pauses the game." << endl << endl;
	cout << "You shall eat all the breadcrumbs on the board in an attempt to win the game" << endl;
	cout << "The Ghosts (" << GHOST << ") will chase you, after 3 times they succeed you will die brutally" << endl;
	cout << "Play as a Pacman (" << PACMAN << ") and win the game." << endl;
	cout << "Good Luck!!!" << endl << endl;
}

//Prints the menu and returns the chosen option
int Menu::menu()
{
	int res;
	do {
		cout << "Please choose an option:" << endl;
		cout << "(" << GameOptions::COLORGAME << ") Start a new game" << endl << "(" << GameOptions::UNCOLORGAME << ") Start a new game without colors" << endl << "(" << GameOptions::INSTRUCTIONS << ") Present instructions and keys" << endl << "(" << GameOptions::EXIT << ") EXIT" << endl;
		cin >> res;
		clear_screen();
	} while (!IsInGameOptions(res));
	return res;
}

bool Menu::IsInGameOptions(int num)
{
	return num == (int)GameOptions::COLORGAME || num == (int)GameOptions::EXIT || num == (int)GameOptions::INSTRUCTIONS || num == (int)GameOptions::UNCOLORGAME;
}

void Menu::ChooseGameForm(bool withcolor,PlayMode mode)
{
	int choose = 0;
	do {
		cout << "Please choose game form:" << endl;
		cout << "(1) In order of maps" << endl << "(2) Specific map" << endl;
		cin >> choose;
		clear_screen();
	} while (choose != 1 && choose != 2);
	if (choose == 1)
		GamePlay::PlayByMaps(withcolor,mode);
	else
		GamePlay::PlaySpecificMap(withcolor,mode);
}
