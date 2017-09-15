#include "PathPlanning/Obstacle.h"
#include <cmath>


Obstacle::Obstacle()
{
}


Obstacle::~Obstacle()
{
}

Obstacle::Obstacle(float r2Posx, float r2Posy, float angleFromE, float angleRangeSensors, float dist)
{
	float xpos = r2Posx + sin(angleFromE)*dist;
	float ypos = r2Posy + cos(angleFromE)*dist;
	float angle = (3.141592653f / 2.f) - angleFromE;
	float borderlineWidth = 2* tan(angleRangeSensors/2) * dist;
	float height = 0;
	float width = 0;
	width = sin(angle)*borderlineWidth;
	height = cos(angle)*borderlineWidth;
	if(sin(angleFromE) == 0) {
		width = 1;
	}
	if (sin(angleFromE) == 0) {
		height = 1;
	}
	top = ypos - height * .5f;
	bottom = ypos + height * .5f;
	left = xpos - width * .5f;
	right = xpos + width * .5f;
	if (dist <= 13.f) {
		if (cos(angleFromE) > 0) {
			if (sin(angleFromE) <= 1 && sin(angleFromE) >= 0) {
				top -= 13.f;
				right += 13.f;
				if (sin(angleFromE) == 0) {
					bottom += 13.f;
				}
			}
			if (sin(angleFromE) >= -1 && sin(angleFromE) <= 0) {
				bottom -= 13.f;
				right += 13.f;
				if (sin(angleFromE) == 0) {
					top -= 13.f;
				}
			}
		}
		else if (cos(angleFromE) < 0) {
			if (sin(angleFromE) <= 1 && sin(angleFromE) >= 0) {
				top -= 13.f;
				left -= 13.f;
				if (sin(angleFromE) == 0) {
					bottom += 13.f;
				}
			}
			if (sin(angleFromE) >= -1 && sin(angleFromE) <= 0) {
				bottom -= 13.f;
				left -= 13.f;
				if (sin(angleFromE) == 0) {
					top -= 13.f;
				}
			}
		}
		else {
			left -= 13.f;
			right += 13.f;
			if (sin(angleFromE) > 0) {
				top -= 13.f;
			}
			if (sin(angleFromE) < 0) {
				bottom -= 13.f;
			}
		}
	}
	else {
		top -= 13.f;
		bottom -= 13.f;
		right += 13.f;
		left -= 13.f;
	}
	
}

Obstacle::Obstacle(float x, float y, float h, float w)
{
	top = y - h * .5f -13.f;
	bottom = y + h * .5f + 13.f;
	left = x - w * .5f - 13.f;
	right = x + w * .5f + 13.f;
}
