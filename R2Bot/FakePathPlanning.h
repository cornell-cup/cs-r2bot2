#pragma once

#include "JobHandler.h"
#include "Data/UltrasoundData.h"
#include "PathPlanning/Grid.h"
#include "PathPlanning/Dijkstra.h"
#include "PathPlanning/Path.h"
#include "Data/MotorData.h"

class FakePathPlanning
{
		
private:
	Grid g;
	Dijkstra shortPath;
	Path path;
	float goalx;
	float goaly;
	int track;

public:
	FakePathPlanning();
	FakePathPlanning(float w, float h, float scal, float startx, float starty, float startAngle, float goalx, float goaly);
	~FakePathPlanning();
	void sendData(SensorData & data);
};

