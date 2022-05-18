#include "SwapChain.h"
#include "GraphicsEngine.h"

SwapChain::SwapChain()
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device *device = GraphicsEngine::get()->m_d3d_device;//The Swap chain requires a pointer to a d3ddevice, get it from the graphics engine
	DXGI_SWAP_CHAIN_DESC desc; //Pointer to swap chain descirptor, C++ structure that holds values such as handle and size of our window
	ZeroMemory(&desc, sizeof(desc)); //Avoid issues caused by dirty values in the structure, fills the structure with zero
	desc.BufferCount = 1; //Need a front and back buffer, but the front buffer is given by the desktop itself, so we only need one
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //Format which gives us 8bites/1byte for each channel for our pixels
	desc.BufferDesc.RefreshRate.Numerator = 60; //Set refresh rate at 60, but maybe come back here and update it to the screens refresh rate
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = hwnd; //Output to the handle of our window
	desc.SampleDesc.Count = 1; //Number of samples per pixel
	desc.SampleDesc.Quality = 0;
	desc.Windowed = TRUE; //Enable windowed mode


	//Create the swap chain for the window indiciated by the HWND parameter, return as a result so we know it worked
	HRESULT hr= GraphicsEngine::get()->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

	if(FAILED(hr))
	{
		return false;
	}

	//Getting the back buffer
	ID3D11Texture2D* buffer=NULL;
	//Gets the buffer via swap chain pointer, at index 0, getting a texture, and converting it to a render target view
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr))
	{
		return false;
	}

	//After checking, we can create the render target view from the device
	//Gets the resource (our back buffer texture), description of how we want to create the view (can use null, as it is from the buffer we passed), and lastly takes the output value
	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(hr))
	{
		return false;
	}



	return true;
}

bool SwapChain::release()
{
	m_swap_chain->Release();
	delete this;
	return true;
}

bool SwapChain::present(bool vsync)
{
	//The present function needs two parameters, one being sync, determines if presentation is immediate or not
	//Second parameter remains to be implemented, so null is fine for now
	m_swap_chain->Present(vsync, NULL);
	return true;
}
