#pragma once

#include <Windows.h>
#include <NuiApi.h>
#include <iostream>
#include "Sensor.h"

// Path of directory where image file and binary should be stored
#define IMAGE_BMP_PATH "C:\\Users\\CornellCup\\Desktop\\image.bmp"
#define IMAGE_BIN_PATH ".\\image.bin"

class KinectSensor : public Sensor
{
protected:
	/** Data from the sensor */
	void *data;
	/** Obtains the next color image data frame */
	void getColorData();
	/** Obtains the next depth image data frame */
	void getDepthData();
	/** A handle to the color image stream data */
	HANDLE colorStreamHandle;
	/** A handle to the depth image stream data */
	HANDLE depthStreamHandle;
public:
	/** Object representing a Kinect sensor */
	INuiSensor *sensor;
	KinectSensor(std::string name);
	virtual ~KinectSensor();
	bool getSensorData();
};
