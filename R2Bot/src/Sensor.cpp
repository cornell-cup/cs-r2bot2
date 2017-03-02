#include "Sensor.h"

Sensor::Sensor()
{
}

Sensor::Sensor(std::string sensorName) : name(sensorName)
{
}

Sensor::~Sensor()
{
}

std::string Sensor::getName()
{
	return name;
}

bool Sensor::ping()
{
	return false;
}

void Sensor::getData(smap<void*> sensorData)
{
	return;
}