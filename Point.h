#pragma once
#include "io_utils.h"

using namespace std;

class Point 
{
protected:
	int x , y;
	void move(const Keys& key);
public:
	Point(int x, int y);
	Point();
	virtual ~Point() {}
	bool compare(Point &p);
	void draw(char ch) const;
	Point& operator=(const int arr[2]);
	bool operator==(const Point& a) const;
	int GetX() const;
	int GetY() const;
	const Keys OpKey(const Keys& key) const;
};

