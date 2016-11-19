#pragma once

#include <string>
#include <mutex>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WImage>
#include <Wt/WLink>
#include <Wt/WText>
#include <wt/WTimer>
#include <wt/WServer>
#include <wt/WResource>
#include <wt/Http/Response>
#include <wt/WEnvironment>
#include <chrono>

using namespace Wt;

class KinectImageResource : public Wt::WResource
{

public:
	char *_image;
	int _totalSize;		// Total size of the buffer
	int _numBytes;			// Number of bytes stored in the buffer
	std::mutex _imageMutex;

	KinectImageResource(Wt::WObject *parent = 0) : Wt::WResource(parent)
	{
		_totalSize = 0;
		_numBytes = 0;
		_image = new char[_totalSize];
	}

	~KinectImageResource() {
		beingDeleted();
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

	void handleRequest(const Wt::Http::Request& request,
		Wt::Http::Response& response)
	{
		response.setMimeType("image/jpeg");
		_imageMutex.lock();
		response.out().write(reinterpret_cast<const char *>(_image), _numBytes);
		_imageMutex.unlock();
	}
};


class R2Server : public WApplication
{
public:
	R2Server(const WEnvironment& env);
	static int run(int imageWidth, int imageHeight, int argc, char **argv);
	static int _imageWidth;
	static int _imageHeight;
	static KinectImageResource *k;

private:
	WImage *kinectImage;
	WTimer *timer;

	void refreshKinectImage();
};
