#include "Dijkstra.h"
#include <cmath>
#include <queue>
#include <iostream>

using namespace std;

class Compare {

public:
	bool operator()(Coord a, Coord b) {
		return a.d > b.d;
	}
};

Dijkstra::Dijkstra()
{
	goalx = 0;
	goaly = 0;
}


Dijkstra::~Dijkstra()
{
}

Dijkstra::Dijkstra(float x, float y)
{
	goalx = x;
	goaly = y;
}

Path Dijkstra::calcPath(Grid *gri)
{
	Grid g = *gri;
	vector<int> goalInt = g.intPos(goalx, goaly);
	if (goalInt[0] >= g.map.size() || goalInt[1] >= g.map[0].size()) {
		goal = g.R2Pos;
	}
	else {
		goal = g.map[goalInt[0]][goalInt[1]];
	}
	priority_queue <Coord, vector<Coord>, Compare> queue;
	vector<int> intCoords;
	intCoords = g.intPos(g.R2Pos.x, g.R2Pos.y);
	g.map[intCoords[0]][intCoords[1]].d = 0;
	init = g.map[intCoords[0]][intCoords[1]];
	if (init.obst) {
		goal = init;
	}
	queue.push(init);
	Coord head = queue.top();
	vector <vector<int>> neighbors;
	while (!(head.equals(goal) || goal.equals(init))) {
		head = queue.top();
		queue.pop();
		intCoords = g.intPos(head.x, head.y);
		neighbors = g.adjCoords(intCoords[0], intCoords[1]);
		for (vector<int> childcoords : neighbors) {
			int xC = childcoords[0];
			int yC = childcoords[1];
			if (g.map[xC][yC].d == -1) {
				if (g.map[xC][yC].obst) {
					g.map[xC][yC].d = numeric_limits<float>::infinity();
					if (g.map[xC][yC].equals(goal)) {
						goal = init;
					}
				}
				else {
					g.map[xC][yC].d = g.distance(g.map[xC][yC], head) + head.d;
				}
				queue.push(g.map[xC][yC]);
			}
			else {
				g.map[xC][yC].d = fmin(g.map[xC][yC].d, head.d + g.distance(g.map[xC][yC], head));
			}
		}
	}
	head = queue.top();
	return givePath(&g);
}

Path Dijkstra::givePath(Grid *grid)
{
	Grid g = *grid;
	Path moves;
	vector<int> intCoords = g.intPos(goal.x, goal.y);
	goal = g.map[intCoords[0]][intCoords[1]];
	moves.path.push_back(goal);
	vector<vector<int>> neighbors = g.adjCoords(intCoords[0], intCoords[1]);
	Coord current = goal;
	g.map[intCoords[0]][intCoords[1]].track = true;
	int xCC;
	int yCC;
	while (!current.equals(init)) {
		for (vector<int> child : neighbors) {
			xCC = child[0];
			yCC = child[1];
			if (!g.map[xCC][yCC].track && abs(g.map[xCC][yCC].d - abs(current.d - g.distance(g.map[xCC][yCC], current))) < .0001) {
				moves.path.push_back(g.map[xCC][yCC]);
				g.map[xCC][yCC].track = true;
				current = g.map[xCC][yCC];
				break;
			}
		}
		intCoords = g.intPos(current.x, current.y);
		neighbors = g.adjCoords(intCoords[0], intCoords[1]);
		g.map[xCC][yCC].track = true;
	}
	return moves;
}

