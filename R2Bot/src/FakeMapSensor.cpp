#include "PathPlanning/FakeMapSensor.h"

FakeMapSensor::FakeMapSensor()
{
	g = Grid(1000, 1000, 25, 10, 26, 0);
	int numObs = 0 + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (10 - 0)));
	float centerx = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1000 - 0)));
	float centery = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1000 - 0)));
	float height = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1000 - 0)));
	float width = 0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1000 - 0)));
	Obstacle x(centerx, centery, height, width);
	g.addObstacle(x);
}

void FakeMapSensor::addObstacle(float x, float y, float h, float w)
{
	Obstacle n(x, y, h, w);
	g.addObstacle(n);
}
