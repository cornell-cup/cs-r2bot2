#include "Sensor/Kinect1Sensor.h"

#ifdef USE_KINECT1

#define JPEG_QUALITY JPEG_DEFAULT
INuiSensor* sensor;
Kinect1Sensor::Kinect1Sensor(std::string sensorName)
{
	name = sensorName;
	imageWidth = 640;
	imageHeight = 480;
	imageSize = imageWidth * imageHeight * 4 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	imageMutex = new std::mutex();
	bmpImage = new char[imageSize];
	// Initialize image headers
	BITMAPINFOHEADER *bih = reinterpret_cast<BITMAPINFOHEADER *>(bmpImage + sizeof(BITMAPFILEHEADER));
	bih->biSize = sizeof(BITMAPINFOHEADER);				// Size of the header
	bih->biBitCount = 32;								// Bit count
	bih->biCompression = BI_RGB;						// Standard RGB, no compression
	bih->biWidth = imageWidth;							// Width in pixels
	bih->biHeight = -imageHeight;						// Height in pixels
	bih->biPlanes = 1;									// Default
	bih->biSizeImage = imageWidth * imageHeight * 4;	// Image size in bytes
	BITMAPFILEHEADER *bfh = reinterpret_cast<BITMAPFILEHEADER *>(bmpImage);
	bfh->bfType = 0x4D42;                                       // 'M''B', indicates bitmap
	bfh->bfOffBits = bih->biSize + sizeof(BITMAPFILEHEADER);	// Offset to the start of pixel data
	bfh->bfSize = bfh->bfOffBits + bih->biSizeImage;			// Size of image + headers
	colorData = bmpImage + bfh->bfOffBits;
	// End image header initialization
	fipi = new fipImage();
	fipmio_bmp = new fipMemoryIO(reinterpret_cast<BYTE *>(bmpImage), imageSize);
	fipmio_jpg = new fipMemoryIO();
	jpgSize = 0;
	jpgImage = new char[jpgSize];
	NuiCreateSensorByIndex(0, &sensor);
	// Initialize Kinect Sensor
	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH);
	sensor->NuiCameraElevationSetAngle(0);
	//Opens up the image stream so that image frames can be obtained
	HANDLE colorEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
	colorStreamHandle = HANDLE(INVALID_HANDLE_VALUE);
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_DEPTH_MAXIMUM, NUI_IMAGE_STREAM_FRAME_LIMIT_MAXIMUM, colorEventHandle, &colorStreamHandle);
	//Opens up the depth image stream so that depth image frames can be obtained
	HANDLE depthEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
	depthStreamHandle = HANDLE(INVALID_HANDLE_VALUE);
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH, NUI_IMAGE_RESOLUTION_640x480, 0, 2, depthEventHandle, &depthStreamHandle);
}

Kinect1Sensor::~Kinect1Sensor()
{
}

void Kinect1Sensor::getColorData() {
	NUI_IMAGE_FRAME frame;
	//Gets the next image frame
	HRESULT hr = sensor->NuiImageStreamGetNextFrame(colorStreamHandle, 1000, &frame);
	NUI_LOCKED_RECT nlr;
	frame.pFrameTexture->LockRect(0, &nlr, NULL, 0);
	if (nlr.Pitch == 0) {
		std::cout << "No data received from the frame";
	}
	else {
		// Synchronized
		imageMutex->lock();
		memcpy(colorData, nlr.pBits, nlr.size);		// Store the color data for later retrieval

		// Convert bmp to jpeg and save it to memory
		fipmio_bmp->seek(0, 0);
		fipi->loadFromMemory(*fipmio_bmp);
		fipi->convertTo24Bits();
		fipmio_jpg->seek(0,0);
		fipmio_jpg->save(FIF_JPEG, *fipi, JPEG_QUALITY);
		int size_in_bytes = fipmio_jpg->tell();
		if (size_in_bytes > jpgSize) {		// Extend buffer if necessary
			delete jpgImage;
			jpgSize = size_in_bytes;
			jpgImage = new char[jpgSize];
		}
		fipmio_jpg->seek(0, 0);
		fipmio_jpg->read(jpgImage, 1, size_in_bytes);		// Copy image over to jpgImage

		imageMutex->unlock();
	}
	frame.pFrameTexture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(colorStreamHandle, &frame);
}

void Kinect1Sensor::getDepthData() {
	NUI_IMAGE_FRAME frame;
	//Gets the next depth image frame
	HRESULT hr = sensor->NuiImageStreamGetNextFrame(depthStreamHandle, 1000, &frame);
	BOOL nearMode;
	INuiFrameTexture* pTexture;
	// Get the image texture information of the depth image frame
	hr = sensor->NuiImageFrameGetDepthImagePixelFrameTexture(depthStreamHandle, &frame, &nearMode, &pTexture);
	NUI_LOCKED_RECT nlr;
	frame.pFrameTexture->LockRect(0, &nlr, NULL, 0);
	if (nlr.Pitch == 0) {
		std::cout << "No data received from the frame";
	}
	else {
		//Set the minimum and maximum reliable depth pixel indices to consider
        int minDepth = (nearMode ? NUI_IMAGE_DEPTH_MINIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MINIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
		int maxDepth = (nearMode ? NUI_IMAGE_DEPTH_MAXIMUM_NEAR_MODE : NUI_IMAGE_DEPTH_MAXIMUM) >> NUI_IMAGE_PLAYER_INDEX_SHIFT;
		//Iterate through all NUI_DEPTH_IMAGE_PIXEL in the frame, where each NUI_DEPTH_IMAGE_PIXEL provides the player index
		//and depth at that pixel
		const NUI_DEPTH_IMAGE_PIXEL * currentPixel = reinterpret_cast<const NUI_DEPTH_IMAGE_PIXEL *>(nlr.pBits);
		const NUI_DEPTH_IMAGE_PIXEL * endPixel = currentPixel + 154616;
		while (currentPixel < endPixel) {
			if (currentPixel) {
				if ((currentPixel->depth >= minDepth) && (currentPixel->depth <= maxDepth)) {
					std::cout << "Pixel Index: " << currentPixel->playerIndex << " Pixel Depth: " << currentPixel->depth << "\n";
				}
				++currentPixel;
			}
		}
	}
	pTexture->UnlockRect(0);
	pTexture->Release();
	sensor->NuiImageStreamReleaseFrame(depthStreamHandle, &frame);
}

bool Kinect1Sensor::getSensorData() {
	getColorData();
	getDepthData();
	return true;
}

#endif