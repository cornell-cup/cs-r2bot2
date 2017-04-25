#include "Sensor/DrawerSensor.h"
#include "Data/ForwardData.h"

DrawerSensor::DrawerSensor(string port, int baudrate) : Sensor("Drawer Sensor"), conn(std::make_shared<SerialPort>(port, baudrate)),
dataMutex(), dataReceived(), dataToForward() {
	
}

DrawerSensor::~DrawerSensor() {
}

bool DrawerSensor::ping() {
	return conn->isConnected() == 1;
}

void DrawerSensor::fillData(smap<void*>& sensorData) {
	
}