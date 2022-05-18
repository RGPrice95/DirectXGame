#pragma once
#include <d3d11.h>

class DeviceContext;
class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	//Initalize SwapChain for a window
	bool init(HWND hwnd, UINT width, UINT height);
	//Release the swap chain
	bool release();

	bool present(bool vsync);

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_rtv;

private:
	friend class DeviceContext; //So the DeviceContext can get a reference to this
};

