#ifndef PATH_H
#define PATH_H

#include <vector>
#include "Coord.h"

using namespace std;

class Path
{
public:
	Path();
	~Path();

	vector<Coord> path;
};

#endif

