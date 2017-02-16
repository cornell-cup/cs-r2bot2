#pragma once

#include <string>
#include <mutex>
#include <chrono>


class KinectImageResource
{

public:
	char *_image;
	int _totalSize;		// Total size of the buffer
	int _numBytes;			// Number of bytes stored in the buffer
	std::mutex _imageMutex;

	KinectImageResource()
	{
		_totalSize = 0;
		_numBytes = 0;
		_image = new char[_totalSize];
	}

	void setImage(char *newImage, int newLength) {
		_imageMutex.lock();
		if (newLength > _totalSize) {
			delete _image;
			_image = new char[newLength];
			_totalSize = newLength;
		}
		memcpy(_image, newImage, newLength);
		_numBytes = newLength;
		_imageMutex.unlock();
	}

	void handleRequest()
	{
		//response.setMimeType("image/jpeg");
		_imageMutex.lock();
		//response.out().write(reinterpret_cast<const char *>(_image), _numBytes);
		_imageMutex.unlock();
	}
};


class R2Server
{
public:
	R2Server();
	static int run(int imageWidth, int imageHeight, int argc, char **argv);
	static int _imageWidth;
	static int _imageHeight;
	static KinectImageResource *k;

private:

	void refreshKinectImage();
}; 
