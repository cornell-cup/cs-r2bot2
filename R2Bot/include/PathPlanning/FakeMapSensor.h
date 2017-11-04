#pragma once
#ifndef FAKEMAPSENSOR_H
#define FAKEMAPSENSOR_H
#include "Sensor.h"
#include "PathPlanning/Grid.h"

class FakeMapSensor:public Sensor {
public:
	Grid g;
	FakeMapSensor();
	void addObstacle(float x, float y, float h, float w);


};

#endif