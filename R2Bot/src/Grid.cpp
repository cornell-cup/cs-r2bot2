#include "PathPlanning/Grid.h"
#include <cmath>
#include <iostream>

Grid::Grid()
{
}


Grid::~Grid()
{
}

Grid::Grid(float w, float h, float scal, float startx, float starty, float startAngle)
{
	width = w;
	height = h;
	scale = scal;
	r2Angle = startAngle;
	nheight = (int)(h / scal + .5);
	nwidth = (int)(w / scal + .5);
	map = vector<vector <Coord>>(nheight, vector<Coord>(nwidth));
	R2Pos = Coord(startx, starty);
	obstacles = vector <Obstacle>();
	vector<int> intInit = intPos(startx, starty);
	for (int i = 0; i < nheight; i++) {
		for (int j = 0; j < nwidth; j++) {
			map[i][j] = Coord((.5f) * scale + scale * ((float)(j)), (.5f) * scale + scale * ((float)(i)));
		}
	}
	map[intInit[0]][intInit[1]] = R2Pos;
	r2OrigIntX = intInit[0];
	r2OrigIntY = intInit[1];
}

void Grid::addObstacle(Obstacle obst)
{
	obstacles.push_back(obst);
	vector<int> coordsOne;
	vector<int> coordsTwo;
	if (obst.top < 0) {
		obst.top = 0;
	}
	if (obst.bottom > height) {
		obst.bottom = height;
	}
	if (obst.left < 0) {
		obst.left = 0;
	}
	if (obst.right > width) {
		obst.right = width;
	}
	coordsOne = intPos(obst.left, obst.top);
	coordsTwo = intPos(obst.right, obst.bottom);
	for (int t = coordsOne[0]; t <= coordsTwo[0]; t++) {
		for (int u = coordsOne[1]; u <= coordsTwo[1]; u++) {
			map[t][u].obst = true;
		}
	}
}

std::vector<vector<int>> Grid::adjCoords(int x, int y)
{
	vector<vector <int>> adj(0, vector<int>(2));
	vector<int> coords;

	if (x + 1 < nheight) {
		coords.push_back(x + 1);
		coords.push_back(y);
		adj.push_back(coords);
		coords.clear();
		if (y + 1 < nwidth) {
			coords.push_back(x + 1);
			coords.push_back(y + 1);
			adj.push_back(coords);
			coords.clear();
		}
	}
	if (y + 1 < nwidth) {
		coords.push_back(x);
		coords.push_back(y + 1);
		adj.push_back(coords);
		coords.clear();
	}
	if (x - 1 >= 0) {
		coords.push_back(x - 1);
		coords.push_back(y);
		adj.push_back(coords);
		coords.clear();
		if (y + 1 < nwidth) {
			coords.push_back(x - 1);
			coords.push_back(y + 1);
			adj.push_back(coords);
			coords.clear();
		}
		if (y - 1 >= 0) {
			coords.push_back(x - 1);
			coords.push_back(y - 1);
			adj.push_back(coords);
			coords.clear();
		}
	}
	if (y - 1 >= 0) {
		coords.push_back(x);
		coords.push_back(y - 1);
		adj.push_back(coords);
		coords.clear();
		if (x + 1 < nheight) {
			coords.push_back(x + 1);
			coords.push_back(y - 1);
			adj.push_back(coords);
			coords.clear();
		}
	}
	return adj;
}

std::vector<int> Grid::intPos(float x, float y)
{
	vector<int> pos;
	pos.push_back((int)((y - (.5f) * scale) / scale + .5));
	pos.push_back((int)((x - (.5f) * scale) / scale + .5));
	return pos;
}

float Grid::distance(Coord a, Coord b)
{
	return (float)sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

void Grid::updatePos(float x, float y)
{
	vector<int> intInit = intPos(x, y);
	R2Pos = map[intInit[0]][intInit[1]];
	r2OrigIntX = intInit[0];
	r2OrigIntY = intInit[1];
}

