#pragma once

#include <Windows.h>
#include <NuiApi.h>
#include <iostream>
#include "Sensor.h"

class KinectSensor : public Sensor
{
protected:
	/** Object representing a Kinect sensor */
	INuiSensor *sensor;
	/** Data from the sensor */
	void *data;
	/** Obtains the next color data frame */
	void getColorData();
	/** Obtains the next depth data frame */
	void getDepthData();
public:
	KinectSensor(std::string name);
	virtual ~KinectSensor();
	bool getSensorData();
};
