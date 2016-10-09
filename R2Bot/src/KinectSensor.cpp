#include "KinectSensor.h"

KinectSensor::KinectSensor(std::string sensorName)
{
	name = sensorName;
	NuiCreateSensorByIndex(0, &sensor);
	// Initialize Kinect Sensor
	sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH);
	sensor->NuiCameraElevationSetAngle(0);
}

KinectSensor::~KinectSensor()
{
}

/// <summary>
/// Save passed in image data to disk as a bitmap
/// </summary>
/// <param name="pBitmapBits">image data to save</param>
/// <param name="lWidth">width (in pixels) of input image data</param>
/// <param name="lHeight">height (in pixels) of input image data</param>
/// <param name="wBitsPerPixel">bits per pixel of image data</param>
/// <param name="lpszFilePath">full file path to output bitmap to</param>
/// <returns>indicates success or failure</returns>
HRESULT SaveBitmapToFile(BYTE* pBitmapBits, LONG lWidth, LONG lHeight, WORD wBitsPerPixel, LPCWSTR lpszFilePath)
{
	DWORD dwByteCount = lWidth * lHeight * (wBitsPerPixel / 8);

	BITMAPINFOHEADER bmpInfoHeader = { 0 };

	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);  // Size of the header
	bmpInfoHeader.biBitCount = wBitsPerPixel;             // Bit count
	bmpInfoHeader.biCompression = BI_RGB;                    // Standard RGB, no compression
	bmpInfoHeader.biWidth = lWidth;                    // Width in pixels
	bmpInfoHeader.biHeight = -lHeight;                  // Height in pixels, negative indicates it's stored right-side-up
	bmpInfoHeader.biPlanes = 1;                         // Default
	bmpInfoHeader.biSizeImage = dwByteCount;               // Image size in bytes

	BITMAPFILEHEADER bfh = { 0 };

	bfh.bfType = 0x4D42;                                           // 'M''B', indicates bitmap
	bfh.bfOffBits = bmpInfoHeader.biSize + sizeof(BITMAPFILEHEADER);  // Offset to the start of pixel data
	bfh.bfSize = bfh.bfOffBits + bmpInfoHeader.biSizeImage;        // Size of image + headers

																   // Create the file on disk to write to
	HANDLE hFile = CreateFileW(lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Return if error opening file
	if (NULL == hFile)
	{
		return E_ACCESSDENIED;
	}

	DWORD dwBytesWritten = 0;

	// Write the bitmap file header
	if (!WriteFile(hFile, &bfh, sizeof(bfh), &dwBytesWritten, NULL))
	{
		CloseHandle(hFile);
		return E_FAIL;
	}

	// Write the bitmap info header
	if (!WriteFile(hFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &dwBytesWritten, NULL))
	{
		CloseHandle(hFile);
		return E_FAIL;
	}

	// Write the RGB Data
	if (!WriteFile(hFile, pBitmapBits, bmpInfoHeader.biSizeImage, &dwBytesWritten, NULL))
	{
		CloseHandle(hFile);
		return E_FAIL;
	}

	// Close the file
	CloseHandle(hFile);
	return S_OK;
}

std::wstring s2ws(const std::string& s)
{
	int slength = (int)s.length() + 1;
	int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void KinectSensor::getColorData() {
	NUI_IMAGE_FRAME frame;
	HANDLE colorEventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);
	HANDLE colorStreamHandle(INVALID_HANDLE_VALUE);
	//Opens up the image stream so that image frames can be obtained
	sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR, NUI_IMAGE_RESOLUTION_640x480, NUI_IMAGE_DEPTH_MAXIMUM, NUI_IMAGE_STREAM_FRAME_LIMIT_MAXIMUM, colorEventHandle, &colorStreamHandle);
	//Gets the next image frame
	HRESULT hr = sensor->NuiImageStreamGetNextFrame(colorStreamHandle, 1000, &frame);
	NUI_LOCKED_RECT nlr;
	frame.pFrameTexture->LockRect(0, &nlr, NULL, 0);
	if (nlr.Pitch == 0) {
		std::cout << "No data received from the frame";
	}
	else {
		SaveBitmapToFile(nlr.pBits, 640, 480, 32, s2ws("C:\\Users\\CornellCup\\Desktop\\image.bmp").c_str());
	}
	frame.pFrameTexture->UnlockRect(0);
	sensor->NuiImageStreamReleaseFrame(colorStreamHandle, &frame);
}

void KinectSensor::getDepthData() {

}

bool KinectSensor::getSensorData() {
	getColorData();
	getDepthData();
	return true;
}