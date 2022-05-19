#include "AppWindow.h"

// Define a vector of 3 float values, represent x,y,z coordinates
struct vec3
{
	float x, y, z;
};


struct vertex
{
	vec3 position;
};

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	Window::onCreate();
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = this->getClientWindowRect();
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	// Create a list of vertices to draw a triangle
	vertex list[] =
	{
		{-0.5f, -0.5f, 0.0f}, // Pos 1
		{0.0f, 0.5f, 0.0f}, // Pos 2
		{0.5f, -0.5f, 0.0f} // Pos 3
	};

	m_vertex_buffer = GraphicsEngine::get()->createVertexBuffer();
	UINT size_list = ARRAYSIZE(list);

	GraphicsEngine::get()->createShaders();
	void* shader_byte_code = nullptr;
	UINT size_shader = 0;
	GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer->load(list, sizeof(vertex), size_list, shader_byte_code, size_shader);
}

void AppWindow::onUpdate()
{
	Window::onUpdate();
	// Clear the render target between frames
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swap_chain, 0,0,1,1);
	// Set the viewport render target
	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	// Set the default shader in graphics pipeline so that they are visible
	GraphicsEngine::get()->setShaders();
	// Set the vertices to be drawn
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vertex_buffer);
	// Draw the triangle
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleList(m_vertex_buffer->getSizeVertexList(), 0);



	m_swap_chain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_vertex_buffer->release();
	m_swap_chain->release();
	GraphicsEngine::get()->release();
}
