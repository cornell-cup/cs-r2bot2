#ifndef COORD_H
#define COORD_H

#include <string>
#include <vector>

using namespace std;

class Coord
{
	friend inline bool operator==(Coord a, Coord b) {
		if (a.row == b.row && a.column == b.column) {
			return true;
		}
		return false;
	}

public:
	Coord();
	~Coord();
	//Coord(float xcoords, float ycoords);
	Coord(vector<int> &intcoords);
	Coord(int intx, int inty);

	int row;
	int column;
	bool obst;
	float d;
	bool track;
	float obstProb; //for vision information
	float hDist;
	bool init;
	bool goal;

	void setObst(bool val);

	string toString();

	void setDistance(float &change, float val);
};

#endif