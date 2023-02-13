#include "Point.h"


//Draw on point the given char
//Input: char to draw
void Point::draw(char ch) const{
	gotoxy(x, y);
	cout << ch;
	gotoxy(0, 0);
}

Point::Point(int x, int y) : x(x), y(y) {};
Point::Point() : Point(0, 0) {};


Point& Point::operator=(const int arr[2])
{
	x = arr[0];
	y = arr[1];
	return *this;
}

bool Point::operator==(const Point& a) const
{
	return x == a.GetX() && y == a.GetY();
}

 /// <summary>
 /// Compare two points
 /// </summary>
 /// <param name="p">Point to compare</param>
 /// <returns>True if equal else False</returns>
 bool Point::compare(Point &p)
{
	return x == p.GetX() && y == p.GetY();
}

 const Keys Point::OpKey(const Keys& key) const
 {
	 switch (key) {
	 case Keys::UP:
		 return Keys::DOWN;

	 case Keys::DOWN:
		 return Keys::UP;

	 case Keys::LEFT:
		 return Keys::RIGHT;

	 case Keys::RIGHT:
		 return Keys::LEFT;
	 
	 default:
		 return Keys::STOP;
	 }
 }


int Point::GetX() const { return x; }
int Point::GetY() const { return y; }


void Point::move(const Keys& key) {
	switch (key) {
	case Keys::UP: --y; break;
	case Keys::DOWN: ++y; break;
	case Keys::LEFT: --x; break;
	case Keys::RIGHT: ++x; break;
	}
}
