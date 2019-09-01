#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include <d3d11.h>
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>

#include <iostream>

#include "fbxLoader.h"


int main()
{
	std::string pFile("D:\\dev\\modelviewer\\Viewer\\assets\\ss\\SportCar20.FBX");

	//auto scene = OpenFile(pFile);

	//if (!scene)
	//{
	//	std::cout << "Unable to open file " << pFile << std::endl;
	//	return -1;
	//}

    std::cout << "Hello World!\n";

	return 0;
}
