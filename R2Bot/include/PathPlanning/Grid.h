#ifndef GRID_H
#define GRID_H

#include "Coord.h"
#include "Obstacle.h"

#include <vector>

using namespace std;

class Grid
{

private:
	int r2OrigIntX;
	int r2OrigIntY;

public:
	Grid();
	~Grid();
	Grid(float w, float h, float scal, float startx, float starty, float startAngle);

	int nheight;
	float height;
	int nwidth;
	float width;
	float scale;
	Coord R2Pos;
	float r2Angle; //clockwise angle from east that r2 is currently facing
	vector<vector<Coord>> map;
	vector<Obstacle> obstacles;

	void addObstacle(Obstacle obst);

	vector<vector<int>> adjCoords(int x, int y);

	vector<int> intPos(float x, float y);

	float distance(Coord a, Coord b);

	void updatePos(float x, float y);
};

#endif
