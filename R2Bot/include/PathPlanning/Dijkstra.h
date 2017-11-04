#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Coord.h"
#include "Path.h"
#include "Grid.h"

class Dijkstra
{

private:
	Path path;
	Grid *grid;

public:
	Dijkstra(Grid *g);
	Dijkstra();
	~Dijkstra();

	Path calcPath(float goalx, float goaly);

	Path givePath(Grid &g, float goalx, float goaly);


};

#endif

