#include "PathPlanning/Dijkstra.h"
#include <cmath>
#include <queue>
#include <iostream>
#include <limits>
#include <iomanip>

using namespace std;

class Compare {

public:
	bool operator()(Coord* a, Coord* b) {
		return a->d > b->d;
	}
};

Dijkstra::Dijkstra(Grid *g)
{
	grid = g;
	path = Path();
}

Dijkstra::Dijkstra()
{
}

Path Dijkstra::calcPath(float goalx, float goaly)
{
	Grid g = *grid;
	Coord* init;
	if (goalx > g.width || goaly > g.height || goalx < 0 || goaly < 0) {
		return Path();
	}
	else {
		vector<int> goalInt = g.intPos(goalx, goaly);
		if (g.map[goalInt[0]][goalInt[1]]->obst) {
			return Path();
		}
		g.map[goalInt[0]][goalInt[1]]->goal = true;
	}
	priority_queue <Coord*, vector<Coord*>, Compare> queue;
	g.map[g.R2Pos->row][g.R2Pos->column]->d = 0;
	g.map[g.R2Pos->row][g.R2Pos->column]->init = true;
	init = g.map[g.R2Pos->row][g.R2Pos->column];
	if (init->obst) {
		return Path();
	}
	queue.push(init);
	Coord* head = queue.top();
	vector <vector<int>> neighbors;
	while (!(head->goal || init->goal)) {
		head = queue.top();
		queue.pop();
		neighbors = g.adjCoords(head->row, head->column);
		for (vector<int> childcoords : neighbors) {
			int xC = childcoords[0];
			int yC = childcoords[1];
			if ((g.map[xC][yC])->d == -1) {
				if (g.map[xC][yC]->obst) {
					g.map[xC][yC]->d = numeric_limits<float>::infinity();
					if (g.map[xC][yC]->goal) {
						init->goal = true;
					}
				}
				else {
					g.map[xC][yC]->d = g.distance(g.map[xC][yC], head) + head->d;
					queue.push(g.map[xC][yC]);
				}
			}
			else {
				g.map[xC][yC]->d = fmin(g.map[xC][yC]->d, head->d + g.distance(g.map[xC][yC], head));
			}
		}
	}
	head = queue.top();
	path = givePath(g, goalx, goaly);
	return path;
}

Path Dijkstra::givePath(Grid &g, float goalx, float goaly)
{
	Path moves;
	vector<int> intCoords = g.intPos(goalx, goaly);
	Coord *goal = g.map[intCoords[0]][intCoords[1]];
	moves.path.push_back(*goal);
	vector<vector<int>> neighbors = g.adjCoords(intCoords[0], intCoords[1]);
	Coord *current = goal;
	g.map[intCoords[0]][intCoords[1]]->track = true;
	int xCC;
	int yCC;
	while (!current->init) {
		for (vector<int> child : neighbors) {
			xCC = child[0];
			yCC = child[1];
			if (!g.map[xCC][yCC]->obst && !g.map[xCC][yCC]->track && abs(g.map[xCC][yCC]->d - abs(current->d - g.distance(g.map[xCC][yCC], current))) < .0001)
			{
				moves.path.push_back(*(g.map[xCC][yCC]));
				g.map[xCC][yCC]->track = true;
				current = g.map[xCC][yCC];
				break;
			}
		}
		neighbors = g.adjCoords(current->row, current->column);
		g.map[xCC][yCC]->track = true;
	}
	return moves;
}

Dijkstra::~Dijkstra()
{
}