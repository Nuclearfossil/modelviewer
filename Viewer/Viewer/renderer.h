#pragma once

class Renderer
{
public:
	Renderer();
	void Present(bool sync);
	void InitGui();
	void NewFrame();
private:
	float clearColor[4];
};

Renderer* CreateRenderer(HWND hWnd);
void CleanupRender();

void ResizeRenderer(UINT width, UINT height);