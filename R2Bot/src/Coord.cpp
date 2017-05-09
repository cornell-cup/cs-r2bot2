#include "PathPlanning/Coord.h"

Coord::Coord()
{
	x = 0;
	y = 0;
	obst = false;
	d = -1;
	track = false;
	obstProb = 0;
	hDist = 0;
}


Coord::~Coord()
{
}

Coord::Coord(float xcoords, float ycoords)
{
	x = xcoords;
	y = ycoords;
	obst = false;
	d = -1;
	track = false;
	obstProb = 0;
	hDist = 0;
}

void Coord::setObst(bool val)
{
	obst = val;
}

bool Coord::equals(Coord compare)
{
	return x == compare.x && y == compare.y;
}

string Coord::toString()
{
	return to_string(x) + ", " + to_string(y);
}

void Coord::setDistance(float &change, float val)
{
	change = val;
}



