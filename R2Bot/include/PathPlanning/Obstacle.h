#ifndef OBSTACLE_H
#define OBSTACLE_H
class Obstacle
{
public:
	Obstacle();
	~Obstacle();
	Obstacle(float r2Posx, float r2Posy, float angleFromE, float angleRangeSensors, float dist);
	Obstacle(float x, float y, float h, float w);
	
	float top;
	float bottom;
	float right;
	float left;

	/*float xpos;
	float ypos;
	float height;
	float width;*/
};

#endif

