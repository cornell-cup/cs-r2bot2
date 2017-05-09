#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Coord.h"
#include "Path.h"
#include "Grid.h"

class Dijkstra
{
private:
	float goalx;
	float goaly;
	Coord goal;
	Coord init;

public:
	Dijkstra();
	~Dijkstra();

	Dijkstra(float x, float y);

	Path calcPath(Grid *gri);

	Path givePath(Grid *grid);
};

#endif

