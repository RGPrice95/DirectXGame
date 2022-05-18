#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"

GraphicsEngine::GraphicsEngine()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	//D3D Drivers, set from best to worst type
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, //Puts the driver on the hardware (Best Case)
		D3D_DRIVER_TYPE_WARP, //Puts it on the CPU, uses SSE Instructions
		D3D_DRIVER_TYPE_REFERENCE //Like Warp, but has very slow preformance (Worst case)
	};
	UINT num_driver_types = ARRAYSIZE(driver_types); //Gets us the size of the array using C++ Macro
	
	//Indicates what feature level of D3D11 we want to use, the higher level, the more features available
	//In order to use this level, it must be inserted into an array even if only one value, thus the below code
	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;
	ID3D11DeviceContext* m_imm_context;
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels, num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);

		if (SUCCEEDED(res)) //If we successfully create the device, we break the loop; we will break on sucessfully creating a D3D11 device on whatever drive works best, first
			break;

		++driver_type_index;

		if (FAILED(res)) //We could not create a device for any of the driver types, return false
			return false;
	}

	m_imm_device_context =  new DeviceContext(m_imm_context);


	//Gets the instance of the DXGI class, the infrastructure assistant which takes care of lower-level tasks
	//Initalize d3d device, adapter, and factory in order to create the swap chain
	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);


	//m_dxgi_factory->CreateSwapChain();


	return true;
}

bool GraphicsEngine::release()
{
	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();


	m_imm_device_context->release();

	m_d3d_device->Release();
	return true;
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}

DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

//We will always get the address of the same instance, we cannot create any more of these, singleton
GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}
