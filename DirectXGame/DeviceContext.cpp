#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"


DeviceContext::DeviceContext(ID3D11DeviceContext* device_context):m_device_context(device_context)
{
}

DeviceContext::~DeviceContext()
{
}

void DeviceContext::setVertexBuffer(VertexBuffer* vertex_buffer)
{
	// Pass the stide (size of vertex type)
	UINT stride = vertex_buffer->m_size_vertex;
	// Set offset - size in bytes that allows us to set the beginning of our buffer
	UINT offset = 0;

	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

// Allows us to render a list of triangles on the screen
void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);
}


// Set what area or viewport of the already settled render target we want to draw
void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_device_context->RSSetViewports(1, &vp);
}

bool DeviceContext::release()
{
	m_device_context->Release();
	delete this;
	return true;
}

void DeviceContext::clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	// Allows us to set which render target we want to draw, use back buffer of our swap chain
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, NULL);
}
