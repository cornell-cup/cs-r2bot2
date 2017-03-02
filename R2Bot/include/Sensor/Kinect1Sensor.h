#ifndef _R2BOT_KINECT1_SENSOR
#define _R2BOT_KINECT1_SENSOR

#include "Global.h"

#ifdef USE_KINECT1

#include <Windows.h>
#include <NuiApi.h>
#include <iostream>
#include "Sensor.h"
#include <mutex>
#include <FreeImagePlus.h>

class Kinect1Sensor : public Sensor
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

	char *colorData;		// Pointer to the pixel data section of the bmpImage
	char *bmpImage;				// Stores the image as BMP headers + pixel data
	int imageSize;				// Total image size, with headers
	fipMemoryIO *fipmio_bmp;	// fip Memory IO representing the bmp image in memory
	fipMemoryIO *fipmio_jpg;	// fip Memory IO representing the jpg image in memory
	fipImage *fipi;			// FreeImage to copy the bmp to
public:
	/** Object representing a Kinect sensor */
	INuiSensor *sensor;
	Kinect1Sensor(std::string name);
	virtual ~Kinect1Sensor();
	bool getSensorData();

	// The following fields are used for broadcasting the image
	std::mutex *imageMutex;		// Lock over image
	char *jpgImage;				// Stores the image as full JPG image
	int jpgSize;
	int imageWidth;				// Image width in pixels
	int imageHeight;			// Image height in pixels
};

#endif
#endif