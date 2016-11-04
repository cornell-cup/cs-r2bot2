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
	int _length;
	std::mutex _imageMutex;

	KinectImageResource(int imageSize, Wt::WObject *parent = 0) : Wt::WResource(parent)
	{
		_length = imageSize;
		_image = new char[imageSize];
	}

	~KinectImageResource() {
		beingDeleted();
	}

	void setImage(char *newImage, int newLength) {
		_imageMutex.lock();
		memcpy(_image, newImage, newLength);
		_length = newLength;
		_imageMutex.unlock();
	}

	void handleRequest(const Wt::Http::Request& request,
		Wt::Http::Response& response)
	{
		response.setMimeType("image/bmp");
		_imageMutex.lock();
		response.out().write(reinterpret_cast<const char *>(_image), _length);
		_imageMutex.unlock();
	}
};


class R2Server : public WApplication
{
public:
	R2Server(const WEnvironment& env);
	static int run(int imageWidth, int imageHeight, int imageSize, int argc, char **argv);
	static int _imageWidth;
	static int _imageHeight;
	static int _imageSize;
	static KinectImageResource *k;

private:
	WImage *kinectImage;
	WTimer *timer;

	void refreshKinectImage();
};
