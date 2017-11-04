#ifndef OBSTACLE_H
#define OBSTACLE_H
class Obstacle
{
public:
	Obstacle();
	~Obstacle();
	Obstacle(float x, float y, float h, float w);
	Obstacle(float r2Posx, float r2Posy, float angleFromE, float angleRangeSensors, float dist);

	float top;
	float bottom;
	float left;
	float right;
};

#endif

