#pragma once
// Below line inclides d2d1.lib in linking
#pragma comment(lib, "d2d1")

#include "resource.h"
#include <iostream>
#include <d2d1.h>
#include <Windows.h>

#define MS_PER_FRAME 33
#define IMAGE_PATH "C:\\Users\\Osc\\Desktop\\image.bin"

class R2Desktop
{
public:
	R2Desktop();
	~R2Desktop();

	int Run(HINSTANCE, int);

	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);

};
void UpdateImage();