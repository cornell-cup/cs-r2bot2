#include "PathPlanning/Grid.h"
#include <cmath>
#include <iostream>

Grid::Grid()
{
}


Grid::~Grid()
{
}

Grid::Grid(float w, float h, float scal, float startx, float starty, float strtagl)
{
	width = w;
	height = h;
	scale = scal;
	nheight = (int)(h / scal + .5);
	nwidth = (int)(w / scal + .5);
	r2Angle = strtagl;
	map = vector<vector <Coord*>>(nheight, vector <Coord*>(nwidth));
	obstacles = vector <Obstacle>();
	vector<int> intInit = intPos(startx, starty);
	for (int i = 0; i < nheight; i++) {
		for (int j = 0; j < nwidth; j++) {
			map[i][j] = new Coord(i, j);
		}
	}
	R2Pos = map[intInit[0]][intInit[1]];
}

void Grid::addObstacle(Obstacle obst)
{
	obstacles.push_back(obst);
	vector<int> coordsOne;
	vector<int> coordsTwo;
	if (obst.top < 0) {
		obst.top = -1;
	}
	if (obst.bottom > height) {
		obst.bottom = -1;
	}
	if (obst.left < 0) {
		obst.left = -1;
	}
	if (obst.right > width) {
		obst.right = -1;
	}
	coordsOne = intPos(obst.left, obst.top);
	coordsTwo = intPos(obst.right, obst.bottom);
	for (int i = 0; i < map.size(); i++) {
		for (int j = 0; j < map[0].size(); j++) {
			map[i][j]->d = -1;
			map[i][j]->track = false;
			map[i][j]->init = false;
		}
	}
	for (int t = coordsOne[0]; t <= coordsTwo[0]; t++) {
		for (int u = coordsOne[1]; u <= coordsTwo[1]; u++) {
			map[t][u]->obst = true;
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
	if (x >= 0) {
		pos.push_back((int)((y - (.5f) * scale) / scale + .5));
	}
	if (y >= 0) {
		pos.push_back((int)((x - (.5f) * scale) / scale + .5));
	}
	
	return pos;
}

float Grid::distance(Coord *a, Coord *b)
{
	if (abs(a->row - b->row) > 0 && abs(a->column - b->column) > 0) {
		return (float)sqrt(2 * pow((scale), 2));
	}
	return scale;
}

void Grid::updatePos(int x, int y)
{
	R2Pos = map[x][y];
}

