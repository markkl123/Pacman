#pragma once
#include "ConstLib.h"

class Frame
{
  Keys pacman,fruit,ghosts[4];
  int fruit_value = -1;
  int Fruit_X = -1, Fruit_Y = -1;
public:
	Frame(Keys pacman, Keys fruit, Keys ghosts[4]);
    Frame();
    int getFruitX();
    int getFruitY();
    char getPac() { return (char)pacman;}
    char getFruit() { return (char)fruit;}
    int getFruitVal() { return fruit_value;}

    char* getGhosts();

    void setFruitXY(int x, int y);
    void setGhostByIndex(Keys key,int index);
    void setGhosts(Keys ghosts[4]);
   	void setPac(Keys _pacmen){pacman = _pacmen;} 
    void setFruitVal(int val){fruit_value = val;} 
    void setFruit(Keys _fruit) {fruit = _fruit;}
};

