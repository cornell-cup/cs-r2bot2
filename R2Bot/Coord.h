#ifndef COORD_H
#define COORD_H

#include <string>

using namespace std;

class Coord
{
public:
	Coord();
	~Coord();
	Coord(float xcoords, float ycoords);

	float x;
	float y;
	bool obst;
	float d;
	bool track;
	float obstProb; //for vision information
	float hDist;

	void setObst(bool val);

	bool equals(Coord compare);

	string toString();

	void setDistance(float &change, float val);
};

#endif