#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();
	//Initalize the Graphics engine and DX11 Device
	bool init();
	//Release the resources loaded
	bool release();


public: 
	SwapChain* createSwapChain(); //Graphics Engine will create the instance of our swap chain
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();

public:
	bool createShaders();
	bool setShaders();
	void getShaderBufferAndSize(void** bytecode, UINT* size);

public:
	static GraphicsEngine* get(); //Make this a singleton


private:
	//Immediate device output
	DeviceContext* m_imm_device_context; //Store our context instance by adding a immediate context attribute

private:
	//Ouput parameters for D3DDevice
	ID3D11Device* m_d3d_device; //Direct X Device Pointer
	D3D_FEATURE_LEVEL m_feature_level; //Feature level
private:
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	ID3D11DeviceContext* m_imm_context;

private:
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* m_vs = nullptr;
	ID3D11PixelShader* m_ps = nullptr;

private:
	//A friend class in C++ can access the private and protected members of the class in which it is declared as a friend. 
	//Use it in this case so that the swapchain/vertexbuffer/etc. class can access what it needs from the graphics engine, without having to expose things
	friend class SwapChain;
	friend class VertexBuffer;
};

