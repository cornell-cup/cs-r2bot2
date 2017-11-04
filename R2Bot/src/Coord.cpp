#include "PathPlanning/Coord.h"

Coord::Coord()
{
	row = 0;
	column = 0;
	obst = false;
	d = -1;
	track = false;
	obstProb = 0;
	hDist = 0;
	init = false;
	goal = false;
}


Coord::~Coord()
{
}

//Coord::Coord(float xcoords, float ycoords)
//{
//	x = xcoords;
//	y = ycoords;
//	row = 0;
//	column = 0;
//	obst = false;
//	d = -1;
//	track = false;
//	obstProb = 0;
//	hDist = 0;
//	init = false;
//	goal = false;
//}

Coord::Coord(int intx, int inty)
{
	row = intx;
	column = inty;
	obst = false;
	d = -1;
	track = false;
	obstProb = 0;
	hDist = 0;
	init = false;
	goal = false;
}

Coord::Coord(vector<int> &intcoords)
{
	row = intcoords[0];
	column = intcoords[1];
	obst = false;
	d = -1;
	track = false;
	obstProb = 0;
	hDist = 0;
	init = false;
	goal = false;
}

void Coord::setObst(bool val)
{
	obst = val;
}

string Coord::toString()
{
	return row + ", " + column;
}

void Coord::setDistance(float &change, float val)
{
	change = val;
}



