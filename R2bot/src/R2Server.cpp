#include "R2Server.h"

#define BYTES_PER_PIXEL 4

// Initialize static variables
int R2Server::_imageWidth;
int R2Server::_imageHeight;
int R2Server::_imageSize;
KinectImageResource *R2Server::k = nullptr;

R2Server::R2Server(const WEnvironment& env) : WApplication(env)
{
	setTitle("R2Server");
	
	// Add Widgets to the webpage
	kinectImage = new WImage(root());
	kinectImage->setWidth(640);
	kinectImage->setHeight(480);
	kinectImage->setId("img");

	// Timer event to update image on page
	timer = new WTimer(root());
	timer->setInterval(33);
	timer->timeout().connect(this, &R2Server::refreshKinectImage);
	timer->start();
}

void R2Server::refreshKinectImage()
{
	doJavaScript("document.getElementById('img').src = 'kinectImage?t=' + new Date().getTime()");
}

WApplication *createApplication(const WEnvironment& env)
{
	if (R2Server::k == nullptr) {
		R2Server::k = new KinectImageResource(R2Server::_imageSize);
		env.server()->addResource(R2Server::k, "/kinectImage");
	}
	return new R2Server(env);
}

int R2Server::run(int imageWidth, int imageHeight, int imageSize, int argc, char **argv)
{
	R2Server::_imageWidth = imageWidth;
	R2Server::_imageHeight = imageHeight;
	R2Server::_imageSize = imageSize;
	return WRun(argc, argv, &createApplication);
}
