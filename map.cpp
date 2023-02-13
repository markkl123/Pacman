#include "map.h"

//Constractor
//Input: Colors: with(True) without(False)
Map::Map(int Xmapsize,int Ymapsize, char initmap[25][80],int pacinit[2], int Ghostsnum,int ghost_init[4][2],int BREADCRUMBS_AMOUNT,int init_Score[2],WISDOM dificulty, bool WithColor, int lives,int pointGame)
{
	BreadCrumbs = BREADCRUMBS_AMOUNT;
	if (WithColor)
		setTextColor(Color::RED);
	Pac.SetPacmanInit(pacinit);
	Init_Score[0] = init_Score[0];
	Init_Score[1] = init_Score[1];
	this->GhostNum = Ghostsnum;
	
	Point helper;
	for (int i = 0; i < GhostNum; i++)
	{

		helper = ghost_init[i];
		Ghosts[i] = Ghost(helper, dificulty);
		Ghosts[i]._init_location();
	}
	Xsize = Xmapsize;
	Ysize = Ymapsize;
	
	this->WithColor = WithColor;
	Pac.SetColor(WithColor);
	Reset_Map(Xmapsize, Ymapsize, initmap);
	Pac.updatePoints(pointGame);
	for (int i = 0; i < 3 - lives; i++)
		Pac.updateLive();
	print();
}

Map::~Map() {
	if (WithColor)
		setTextColor(Color::WHITE);
}

//Check if pacman eats food
void Map::CheckForFood()
{
	if (map[Pac.GetY()][Pac.GetX()] == BREADCRUMBS)
	{
		BreadCrumbs--;
		map[Pac.GetY()][Pac.GetX()] = SPACE;
		Pac.updatePoints();
		Draw_Points();
	}
	if (fruit.IsVisible() && (Point)Pac == (Point)fruit)
	{
		Pac.updatePoints(fruit.GetValue());
		Draw_Points();
		fruit.SetVisible(false);
		Pac.drawPac();
	}
}

int* Map::Get_looseRec()
{
	return looseRec;
}

int Map::GetBreadCrumbs() const
{
	return BreadCrumbs;
}

bool Map::IsValidKey(char Key) const
{
	string keys = "wWxXaAdDsS";
	return Key == (int)Keys::ESC || (keys.find(Key)>=0 && keys.find(Key) < keys.length());
}

//Checks if pacman killed by a Ghosts, if so update live and init locations
//Input: Current pacman last key
bool Map::CheckForGhost(Keys& Key)
{
	int i,j;
	if (fruit.IsVisible())
		for (i = 0; i < GhostNum; i++)
			if ((Point)Ghosts[i] == (Point)fruit)
			{
				fruit.SetVisible(false);
				Ghosts[i].DrawGhost();
				break;
			}
	for (i = 0; i < GhostNum; i++)
		if (Pac == Ghosts[i])
		{
			Pac.updateLive();
			Draw_Lives();
			if (fruit.IsVisible())
				fruit.draw(map[fruit.GetY()][fruit.GetX()] == UNVISITED ? SPACE : map[fruit.GetY()][fruit.GetX()]);
			fruit.SetVisible(false);
			
			if (Pac.Get_Lives() > 0)
			{
				for (j = 0; j < 2; j++) {
					Pac.draw(' ');
					Sleep(200);
					Pac.drawPac();
					Sleep(200);
				}
				for (j = 0; j < GhostNum; j++)
				{
					Ghosts[j].draw(map[Ghosts[j].GetY()][Ghosts[j].GetX()] == UNVISITED ? SPACE : map[Ghosts[j].GetY()][Ghosts[j].GetX()]);
					Ghosts[j]._init_location();
				}
				Pac._init_location();
				PrintGhosts();
				for (j = 0; j < 2; j++) {
					Pac.draw(' ');
					Sleep(200);
					Pac.drawPac();
					Sleep(200);
				}
				Sleep(500);
				do
				{
					Key = (Keys)toupper(_getch());
					if (Key == Keys::ESC)
						Pause();
				} while (!IsValidKey((char)Key) || Key == Keys::ESC);
			}
			for (j = 0; looseRec[j] != 0; j++);
			looseRec[j] = time;
			return true;
		}
	return false;
}

int Map::Get_time() { return time; }

void Map::UpTime() { time++; }

void Map::MoveGhostsByFrame(Frame& curr)
{
	Keys currKey;
	for (int i = 0; i < GhostNum; i++)
	{
		currKey = (Keys)(curr.getGhosts()[i]);
		Ghosts[i].draw(map[Ghosts[i].GetY()][Ghosts[i].GetX()] == UNVISITED ? SPACE : map[Ghosts[i].GetY()][Ghosts[i].GetX()]);
		Ghosts[i].Move(map, Xsize, Ysize,currKey);
	}

	PrintGhosts();
}

void Map::MoveGhosts()
{
	Keys rec[4];
	for (int i = 0; i < GhostNum; i++)
	{
		Ghosts[i].draw(map[Ghosts[i].GetY()][Ghosts[i].GetX()] == UNVISITED ? SPACE : map[Ghosts[i].GetY()][Ghosts[i].GetX()]);
		rec[i] = Ghosts[i].MoveGhost(map, Xsize, Ysize, Pac);
	}

/*	for (int i = 0; i < GhostNum; i++)
		for (int j = 0; j < GhostNum; j++)
			if (i < j && (Point)Ghosts[i] == (Point)Ghosts[j])
			{
				Ghosts[j].MoveGhost(map, Xsize, Ysize, Pac);
				Ghosts[j].MoveGhost(map, Xsize, Ysize, Pac);
			}*/
			
	Record[time].setGhosts(rec);
	PrintGhosts();
}

vector<Frame>& Map::getRecord()
{
	return Record;
}

void Map::SetRecord(vector<Frame> R)
{
	Record = R;
}

void Map::MovebyFrameinithelper()
{
	Pac.updateLive();
		Draw_Lives();
		if (fruit.IsVisible())
			fruit.draw(map[fruit.GetY()][fruit.GetX()] == UNVISITED ? SPACE : map[fruit.GetY()][fruit.GetX()]);
		
		if (Pac.Get_Lives() > 0)
		{
			for (int j = 0; j < GhostNum; j++)
			{
				Ghosts[j].draw(map[Ghosts[j].GetY()][Ghosts[j].GetX()] == UNVISITED ? SPACE : map[Ghosts[j].GetY()][Ghosts[j].GetX()]);
				Ghosts[j]._init_location();
			}
			Pac._init_location();
			PrintGhosts();
		}
}

void Map::MovebyFrame(Frame& curr, int loose[3],bool silent)
{
	map[Pac.GetY()][Pac.GetX()] = SPACE;
	bool prevFruit = fruit.IsVisible();
	int j;
	bool helper = true;
	for (j = 0; j < GhostNum; j++)
		helper = helper && !(Ghosts[j] == Pac);
	if ((silent && loose[3 - Pac.Get_Lives()] == time) || !helper)
	{
		if (silent)
		{
			for (j = 0, helper = true; j < GhostNum; j++)
				helper = helper && !(Ghosts[j] == Pac);
			if (helper)
			{
				helper = true;
				Pac.Move(map, Xsize, Ysize, (Keys)curr.getPac());
				for (j = 0, helper = true; j < GhostNum; j++)
					helper = helper && !(Ghosts[j] == Pac);
				if (helper)
					throw "test failed";
			}
		}
		MovebyFrameinithelper();
		return;
	}

	Pac.Move(map, Xsize, Ysize, (Keys)curr.getPac());
	if (!silent)
		for (j = 0; j < GhostNum; j++)
			if (Ghosts[j] == Pac)
			{
				MovebyFrameinithelper();
				return;
			}

	fruit.draw(map[fruit.GetY()][fruit.GetX()] == UNVISITED ? SPACE : map[fruit.GetY()][fruit.GetX()]);

	fruit.SetVisible(curr.getFruitVal() != -1);
	if (fruit.IsVisible())
	{
		if (!prevFruit)
		{
			fruit.SetXY(curr.getFruitX(), curr.getFruitY());
			fruit.SetValue(curr.getFruitVal());
		}
		fruit.Move(map, Xsize, Ysize, (Keys)curr.getFruit());
		fruit.drawFruit();
	}
	Pac.drawPac();
	CheckForFood();
}

//Moves the pacman and preform the needed changes
void Map::Move(Keys& Key)
{
	map[Pac.GetY()][Pac.GetX()] = SPACE;

	if (CheckForGhost(Key))
		return;
	bool prevFruit = fruit.IsVisible();

	Record[time].setPac(Key);

	Pac.Move(map, Xsize, Ysize, Key);
	if (WithColor)
		setTextColor(Color::LIGHTRED);
	Record[time].setFruit(fruit.MoveFruit(map, Xsize, Ysize));
	if (WithColor)
		setTextColor(Color::RED);

	if (fruit.IsVisible())
	{
		if (!prevFruit)
			Record[time].setFruitXY(fruit.GetX(), fruit.GetY());

		Record[time].setFruitVal(fruit.GetValue());
	}

	CheckForFood();
	CheckForGhost(Key);
}

void Map::Reset_Map(int Xmapsize, int Ymapsize, char initmap[25][80])
{
	for (int y = 0; y < Ymapsize; y++)
		for (int x = 0; x < Xmapsize; x++)
			map[y][x] = initmap[y][x];
}

//Prints map
void Map::print() const
{
	gotoxy(0, 0);
	int x,y;
	for (y = 0; y < Ysize; y++)
	{
		for (x = 0; x < Xsize; x++)
		{

			if (WithColor && map[y][x] == WALL)
				setTextColor(Color::BLUE);

			cout << ((map[y][x] == UNVISITED) ? SPACE : map[y][x]);
			if (WithColor)
				setTextColor(Color::RED);
		}
		cout << endl;
	}
	Pac.drawPac();
	PrintGhosts();
	Draw_Lives();
	Draw_Points();
}

void Map::PrintGhosts() const
{
	if (WithColor)
		setTextColor(Color::BROWN);
	for (int i = 0; i < GhostNum; i++)
	{
		Ghosts[i].DrawGhost();
	}
	if (WithColor)
	setTextColor(Color::RED);
}

void Map::PrintFruit() const
{
	if (WithColor)
		setTextColor(Color::LIGHTRED);
	fruit.drawFruit();
	if (WithColor)
		setTextColor(Color::RED);
}

int Map::Get_Pacman_gamepoints() const
{
	return Pac.Get_GamePoints();
}
int Map::Get_Pacman_lives() const
{
	return Pac.Get_Lives();
}

//Returns the character on the given cordinate
char Map::get_Point_Visual(int x, int y) const
{
	return map[y][x];
}

//Pause the game until given ESC key
void Map::Pause() const
{
	clear_screen();
	cout << "The game has paused, Please press Escape to continue..";
	while (_getch() != (int)Keys::ESC);
	clear_screen();
	print();
	Sleep(500);
}

void Map::Draw_Lives() const
{
	if (WithColor)
		setTextColor(Color::GREEN);
	gotoxy(Init_Score[0], Init_Score[1]); cout << "Lives:" << Pac.Get_Lives();
	for (int i = 0; i < 14 - (int)to_string(Pac.Get_Lives()).length(); i++)
		cout << " ";
	if (WithColor)
		setTextColor(Color::RED);
}

void Map::Draw_Points() const
{
	if (WithColor)
		setTextColor(Color::GREEN);
	gotoxy(Init_Score[0], Init_Score[1] + 1); cout << "Points:" << Pac.Get_GamePoints();
	for (int i = 0; i < 13 - (int)to_string(Pac.Get_GamePoints()).length(); i++)
		cout << " ";
	gotoxy(Init_Score[0], Init_Score[1] + 2);
	for (int i = 0; i < 20; i++)
		cout << " ";
	if (WithColor)
		setTextColor(Color::RED);
}