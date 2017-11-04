#ifndef GRID_H
#define GRID_H

#include "Coord.h"
#include "Obstacle.h"

#include <vector>

using namespace std;

class Grid
{
public:
	Grid();
	~Grid();
	Grid(float w, float h, float scal, float startx, float starty,float startangle);

	int nheight;
	float height;
	float r2Angle;
	int nwidth;
	float width;
	float scale;
	Coord *R2Pos;
	vector<vector<Coord*>> map;
	vector<Obstacle> obstacles;

	void addObstacle(Obstacle obst);

	vector<vector<int>> adjCoords(int x, int y);

	vector<int> intPos(float x, float y);

	float distance(Coord *a, Coord *b);

	void updatePos(int x, int y);
};

#endif
