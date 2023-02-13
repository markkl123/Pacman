#include "GamePlay.h"


using filesystem::directory_iterator;

void GamePlay::PlayByMaps(bool WithColor, PlayMode mode)
{
    vector<string> files;
    string path = filesystem::current_path().string();
    // look for files in the working directory, with the names pacman_*.screen
    for (const auto& file : filesystem::directory_iterator(path))
    {
        const string str = file.path().string();
        if (str.find("pacman_") != string::npos && str.find(".screen") != string::npos)
            files.push_back(str);
    }
    sort(files.begin(), files.end());
    int numOfMaps = files.size();

    int live = 3, gamepoints = 0;
    WISDOM dificulty;
    if (!(mode == PlayMode::Load || mode == PlayMode::Silent))
        dificulty = ChooseDificulty();
    else
        dificulty = WISDOM::NOVICE;

    ifstream f;
    string line;

    //Creates maps from the incoming files
    for (string i : files)
    {
        int ySize = 0, xSize = 0;
        int numOfGhost = 0, breadcrumb = 0, numOfPac = 0;
        bool livesInFirstLine = false;
        int pacPoss[2] = { 0 };
        int livesPoss[2] = { 0 };
        char board[25][80] = { 0 };
        int Ghost[4][2] = { 0 };

        f.open(i, ios::in);
        if (!f) { cout << "ERROR" << endl; throw("Map allocation error"); }


        getline(f, line);
        //check if the lives information in the first line
        livesInFirstLine = checkForLiveInformationInFirstLine(line, xSize, livesPoss);
        createMapFromFile(line, board, pacPoss, livesPoss, breadcrumb, ySize, xSize, Ghost, numOfGhost, livesInFirstLine, numOfPac);

        while (!f.eof())
        {
            getline(f, line);
            createMapFromFile(line, board, pacPoss, livesPoss, breadcrumb, ySize, xSize, Ghost, numOfGhost, livesInFirstLine, numOfPac);
        }
        f.close();


        validGame(numOfPac, numOfGhost, xSize, ySize, pacPoss, Ghost);

        if (live <= 0)
            break;

        if (mode == PlayMode::Silent)
            cout.setstate(ios_base::failbit);
        Map map(xSize, ySize, board, pacPoss, numOfGhost, Ghost, breadcrumb, livesPoss, dificulty, WithColor, live, gamepoints);
        int loose_time[3], RecEndTime = 0;
        bool RecIsWin = false;
        if (mode == PlayMode::Load || mode == PlayMode::Silent)
        {
            returnRes(i, loose_time, RecEndTime, RecIsWin);
            PlayByRec(map, importMap(i), loose_time, mode == PlayMode::Silent);
        }
        else
        {
            Play(map);
        }
        cout.clear();
        gamepoints = map.Get_Pacman_gamepoints();
        live = map.Get_Pacman_lives();
        if (mode == PlayMode::Silent && (map.Get_time() != RecEndTime || RecIsWin != (live > 0)))
            throw "test failed";


        if (mode == PlayMode::Save)
            exportMap(i, map.getRecord(), map.Get_looseRec(), 3 - map.Get_Pacman_lives(), map.Get_time(), numOfGhost);
    }
    if (mode == PlayMode::Silent)
        throw "passed";
    else if (live <= 0)
        cout << "Game Over!!!" << endl;
    else
        cout << "Winner winner chicken dinner!!!" << endl;

}

PlayMode GamePlay::strtomode(char** strs, int size)
{
    if (strcmp(strs[1], "-load") == 0)
    {
        if (size == 2)
            return PlayMode::Load;
        else if (size == 3 && strcmp(strs[2], "-silent") == 0)
            return PlayMode::Silent;
    }
    else if (size == 2 && strcmp(strs[1], "-save") == 0)
        return PlayMode::Save;
    cout << "Usage: " << strs[0] << "[-save][-load | -load -silent]" << endl;
    cout << "[-save] runs the game normally and saving it" << endl;
    cout << "[-load] runs the recording of the last game" << endl;
    cout << "[-load -silent] checks if the last record of the game is correct" << endl;
    throw "Please try again.";
}

void GamePlay::createMapFromFile(string line, char Map[25][80],int pacPoss[2],int livesPoss[2],int& breadcrumb,int& ySize, int xSize,int Ghost[4][2],int& numOfGhost,bool livesInFirstLine,int& numOfPac)
{
    
        insertData(line, Map[ySize],pacPoss,livesPoss, Ghost,numOfGhost,breadcrumb,ySize,xSize,numOfPac);
        ySize++;
        
}
    

void GamePlay::insertData(string line, char lineCopy[80],int pacPoss[2],int livesPoss[2], int Ghost[4][2],int& numOfGhost,int& breadcrumb, int ySize,int xSize,int& numOfPac)
{
    if(ySize > 25){throw "Wrong map size";}
    int i = 0;

    for(char const ch:line)
    {
        switch(ch)
        {
          case ' ':
                lineCopy[i] = BREADCRUMBS;
                breadcrumb++;
            break;
        case '#':
                lineCopy[i] = WALL;
            break;
            case '@':
                lineCopy[i] = SPACE;
                pacPoss[0] = i;
                pacPoss[1] = ySize;
                numOfPac++;
                break;
            case '%':
                lineCopy[i] = UNVISITED;
                break;
            case '&':
                lineCopy[i] = UNVISITED;
                livesPoss[0] = i;
                livesPoss[1] = ySize;
                break;
            case '$':
                lineCopy[i] = BREADCRUMBS;
                breadcrumb++;
                Ghost[numOfGhost][0] = i;
                Ghost[numOfGhost][1] = ySize;
                numOfGhost++;
                break;
            default:
                lineCopy[i] = SPACE;
            }
    i++;
    }
    lineCopy[i] = '\0';
}

void GamePlay::validGame(int numOfPac, int numOfGhost,int xSize, int ySize, int pacPoss[2],int Ghost[4][2])
{
    if(numOfPac > 1 || numOfPac == 0){throw "wrong map - Too many pacmans";}\
    if(numOfGhost > 4){throw "Wrong map - Too many Ghosts ";}
    if(xSize > 80) {throw "Wrong map size ";}
    if(pacPoss[0] > xSize || pacPoss[1] > ySize){throw "Invalid pacmen possition";}
    
    for(int i = 0; i <numOfGhost; i++)
    {
       if( Ghost[i][0] > xSize || Ghost[i][1] > ySize){throw "Invalid ghosts possition";}
    }
}

bool GamePlay::checkForLiveInformationInFirstLine(string firstLine,int& xSize,int livesPoss[2])
{
    
    xSize = (int)firstLine.size();
    if(xSize==string::npos)
        xSize = 0;
    
    int livesInformationPoss = (int)firstLine.find("&");
    if(livesInformationPoss==string::npos)
        return false;
    
    livesPoss[0] = livesInformationPoss;
    livesPoss[1] = 0;

    if(livesInformationPoss + 20 > xSize)
    {
        xSize = livesInformationPoss + 20;
        return true;
    }
    return false;
}

void GamePlay::PlayByRec(Map& board, vector<Frame> Rec, int loose_time[3], bool silent)
{
    int GhostSpeed = 0;
    hideCursor();

    while (board.GetBreadCrumbs() > 0 && board.Get_Pacman_lives() > 0)
    {
        if (silent && (board.Get_time() == Rec.size()))
            throw "test failed";
        if (GhostSpeed == 1)
        {
            board.MoveGhostsByFrame(Rec[board.Get_time()]);
            GhostSpeed = 0;
        }
        else { GhostSpeed = 1; }

        board.MovebyFrame(Rec[board.Get_time()], loose_time, silent);
        if (!silent)
            Sleep(200);
        board.UpTime();
    }
    clear_screen();
}

//Plays pacman game
//Input: Map object
void GamePlay::Play(Map &board)
{
	int GhostSpeed = 0;
	Keys c, c_helper;
	hideCursor();
	do {
		c = (Keys)toupper(_getch());
	} while (!board.IsValidKey((char)c));

	while (board.GetBreadCrumbs() > 0 && board.Get_Pacman_lives() > 0)
	{
        board.getRecord().push_back(Frame());
		if (GhostSpeed == 1)
		{
			board.MoveGhosts();
			GhostSpeed = 0;
		}
		else { GhostSpeed = 1; }

		board.Move(c);
		Sleep(200);

		if (_kbhit())
		{
			c_helper = (Keys)toupper(_getch());
			if (c_helper == Keys::ESC)
				board.Pause();
			else if (board.IsValidKey((char)c_helper))
				c = c_helper;
		}
        board.UpTime();
	}
	clear_screen();
}

WISDOM GamePlay::ChooseDificulty()
{
	int choose = 0;
	WISDOM res = WISDOM::BEST;
	do {
		cout << "Please choose dificulty:" << endl;
		cout << "(1) Best" << endl << "(2) Good" << endl << "(3) Novice" << endl;
		cin >> choose;
		clear_screen();
	} while (choose < 1 && choose>3);
	switch (choose)
	{
	case 1:
		res = WISDOM::BEST;
		break;
	case 2:
		res = WISDOM::GOOD;
		break;
	case 3:
		res = WISDOM::NOVICE;
		break;
	}
	return res;
}

void GamePlay::PlaySpecificMap(bool WithColor, PlayMode mode)
{
    string map_Path;
    do
    {
    cout<<"Please enter the Map's path: "<<endl;
        cin >> map_Path;
        clear_screen();
    } while (map_Path.find(".screen") == string::npos);

    WISDOM dificulty = ChooseDificulty();

    int ySize = 0, xSize = 0;
    int numOfGhost = 0, breadcrumb = 0;
    bool livesInFirstLine = false;
    int pacPoss[2] = { 0 };
    int livesPoss[2] = { 0 };
    char board[25][80] = { 0 };
    int Ghost[4][2] = { 0 };
    int numOfPac = 0;
    ifstream f;
    string line;
        
    f.open(map_Path, ios::in);
    if (!f) { cout << "ERROR" << endl; throw("Map allocation error"); }

    
    getline(f, line);
    livesInFirstLine = checkForLiveInformationInFirstLine(line, xSize, livesPoss);
    createMapFromFile(line, board, pacPoss, livesPoss, breadcrumb, ySize, xSize, Ghost, numOfGhost, livesInFirstLine,numOfPac);
    while (!f.eof())
    {
        getline(f, line);
        createMapFromFile(line, board, pacPoss, livesPoss, breadcrumb, ySize, xSize, Ghost, numOfGhost, livesInFirstLine,numOfPac);
    }
    f.close();

    Map map(xSize, ySize, board, pacPoss, numOfGhost, Ghost, breadcrumb, livesPoss, dificulty, WithColor, 3, 0);
    Play(map);

    if (mode == PlayMode::Save)
        exportMap(map_Path, map.getRecord(), map.Get_looseRec(), 3 - map.Get_Pacman_lives(), map.Get_time(), numOfGhost);

    if (map.Get_Pacman_lives() < 1)
        cout << "Game Over!!!" << endl;
    else
        cout << "Winner winner chicken dinner!!!" << endl;	
}



void GamePlay::exportMap(string mapName, vector<Frame> steps, int loseTime[3], int pacLives, int Gettime, int numOfGhost)
{
    size_t pos = mapName.find(".screen");
    pos++;
    ofstream outResult((mapName.substr(0, pos)) + "result");
    ofstream outSteps(mapName.substr(0, pos) + "steps");

    if (pacLives == 3)
        outResult << "The pacman died in" << endl;
    else
        outResult << "The user won the game" << endl;

    for (int i = 0; i < 3 && loseTime[i] != 0; i++)
        outResult << i + 1 << ") " << loseTime[i] << endl;

    outResult << "The pacman's last point of time is: " << Gettime << endl;

    outResult.close();

    outSteps << "#";
    outSteps << "P" << steps[0].getPac();
    char* prevGhost = steps[0].getGhosts();
    if (steps[0].getFruitVal() != -1)
        outSteps << "F" << steps[0].getFruit() << ('0' + steps[0].getFruitVal()) << ('0' + steps[0].getFruitX()) << ('0' + steps[0].getFruitY());
            
    outSteps << "#";

    for (unsigned int i = 1; i < steps.size(); i++)
    {
        outSteps << "\n#";
        if (steps[i - 1].getPac() != steps[i].getPac())
            outSteps << "P" << steps[i].getPac();

        char* tmpGhosts = steps[i].getGhosts();
        if (i % 2 == 1)
        {
            for (int j = 0; j < numOfGhost; j++)
            {
                if (tmpGhosts[j] != prevGhost[j])
                    outSteps << "G" << j << tmpGhosts[j];
            }
            prevGhost = tmpGhosts;
        }
        if (steps[i].getFruitVal() != -1)
        {
            outSteps << "F" << steps[i].getFruit();
            if (steps[i - 1].getFruitVal() == -1)
                outSteps << to_string(steps[i].getFruitVal()) << to_string(steps[i].getFruitX()) << "|" << to_string(steps[i].getFruitY());
        }
        outSteps << "#";
    }
}

void GamePlay::returnRes(string mapName, int loose_time[3], int& RecTime, bool& isWin)
{
    int pos = mapName.find(".screen");
    pos++;
    int helper;
    string res = (mapName.substr(0, pos)) + "result";
    isWin = false;
    ifstream resF;
    resF.open(res, ios::in);
    while (!resF.eof())
    {
        string line;
        getline(resF, line);
       
        if (line.find("1) ")!= string::npos)
        {
            pos = line.find(")");
            pos++;
            helper = stoi(line.substr(pos, line.size()));
            loose_time[0] = helper;
        }
        else if (line.find("2) ")!= string::npos)
        {
            pos = line.find(")");
            pos++;
            helper = stoi(line.substr(pos, line.size()));
            loose_time[1] = helper;
        }
        else if (line.find("3) ") != string::npos)
        {
            pos = line.find(")");
            pos++;
            helper = stoi(line.substr(pos, line.size()));
            loose_time[2] = helper;
        }
        else if (line.find("time is: ") != string::npos)
        {
            pos = line.find(": ");
            pos++;
            RecTime = stoi(line.substr(pos, line.size()));
        }
        else if (line.find("won") != string::npos)
        {
            isWin = true;
        }
    }
    resF.close();
}

vector<Frame> GamePlay::importMap(string mapPath)
{
    size_t pos = mapPath.find(".screen");
    pos++;
    string steps = mapPath.substr(0, pos) + "steps";
    vector<Frame> record;
    ifstream importedMap;
    importedMap.open(steps, ios::in);
    if (!importedMap) { cout << "ERROR" << endl; throw("Map allocation error"); }

    string line;
    getline(importedMap, line);
    Frame frame;
    setFirstFrame(frame);
    createframe(line, frame);
    record.push_back(frame);
    const char* prevGhost = frame.getGhosts();
    while (!importedMap.eof())
    {
        getline(importedMap, line);
        createframe(line, frame);

        if (record.size() % 2 == 0) {
            for (int i = 0; i < 4; i++)
                frame.setGhostByIndex((Keys)prevGhost[i], i);
        }

        else { prevGhost = frame.getGhosts(); }
        record.push_back(frame);
    }
    importedMap.close();
    return record;
}

void GamePlay::setFirstFrame(Frame& frame)
{
    Keys Ghosts[4] = { Keys::STOP,Keys::STOP, Keys::STOP, Keys::STOP };
    frame.setGhosts(Ghosts);
    frame.setFruitVal(-1);
    frame.setGhosts(Ghosts);
}

void GamePlay::createframe(string line, Frame& frame)
{
    string helper1 = "", helper2 = "";
    int j1, j2;
    if (line.find('F') == std::string::npos)
        frame.setFruitVal(-1);
    for (int i = 1; line[i] != '#'; i++)
    {
        if (line[i] == 'P')
        {
            frame.setPac((Keys)line[i + 1]);
            i++;
        }
        if (line[i] == 'G')
        {
            frame.setGhostByIndex((Keys)line[i + 2], stoi(string(1, line[i + 1])));
            i += 2;
        }
        if (line[i] == 'F')
        {
            frame.setFruit((Keys)line[i + 1]);
            if (line[i + 2] != '#')
            {
                frame.setFruitVal(line[i + 2] - '0');

                for (j1 = 3; line[i + j1] != '|'; j1++)
                    helper1 += line[i + j1];
                for (j2 = j1 + 1; line[i + j2] != '#'; j2++)
                    helper2 += line[i + j2];
                frame.setFruitXY(stoi(helper1), stoi(helper2));
                i += j2 - 1;
            }
            else
                i += 1;
        }
    }
}


