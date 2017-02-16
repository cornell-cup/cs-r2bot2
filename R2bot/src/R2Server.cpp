#include "R2Server.h"

#define BYTES_PER_PIXEL 4

// Initialize static variables
int R2Server::_imageWidth;
int R2Server::_imageHeight;
KinectImageResource *R2Server::k = nullptr;

R2Server::R2Server()
{
	//setTitle("R2Server");
	
	// Add Widgets to the webpage

	// Timer event to update image on page
}

void R2Server::refreshKinectImage()
{
	//doJavaScript("document.getElementById('img').src = 'kinectImage?t=' + new Date().getTime()");
}


int R2Server::run(int imageWidth, int imageHeight, int argc, char **argv)
{
	R2Server::_imageWidth = imageWidth;
	R2Server::_imageHeight = imageHeight;
	//return WRun(argc, argv, &createApplication);
	return 1;
}
