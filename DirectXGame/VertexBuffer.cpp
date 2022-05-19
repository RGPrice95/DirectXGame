#include "VertexBuffer.h"
#include "GraphicsEngine.h"

VertexBuffer::VertexBuffer() :m_layout(0), m_buffer(0)
{
}

bool VertexBuffer::load(void* list_vertices, UINT size_vertex, UINT size_list, void* shader_byte_code, UINT size_byte_shader)
{
	// Since this can be used multiple times, release two resources, so new ones can be created for new lists of vertices
	if (m_buffer)m_buffer->Release();
	if (m_layout)m_layout->Release();


	// Set data relative to our buffer
	D3D11_BUFFER_DESC buff_desc = {};
	// Allow GPU to read/write to the buffer, using default
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	// Set size of buffer in bytes
	// Size of buffer will be the product of the size of the vertex, by the number of vertices in the list
	buff_desc.ByteWidth = size_vertex * size_list;
	// Tells DirectX how to bind this buffer to graphics pipeline, bound as a vertex buffer
	buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	// Pass where in memory the list of vertices is located
	init_data.pSysMem = list_vertices;

	m_size_vertex = size_vertex;
	m_size_list = size_list;

	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
		return false;

	// Sucessfully created the buffer, now create the input layout
	// Create descriptor object, which has attributes of our vertex type
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//SEMANTIC NAME - SEMANTIC INDEX - FORMAT - INPUT SLOT - ALIGNED BYTE OFFSET - INPUT SLOT CLASS - INSTANCE DATA STEP RATE
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};



	// Get number of attributes
	UINT size_layout = ARRAYSIZE(layout);
	
	if (FAILED(GraphicsEngine::get()->m_d3d_device->CreateInputLayout(layout, size_layout, shader_byte_code, size_byte_shader, &m_layout)))
		return false;

	return true;
}

UINT VertexBuffer::getSizeVertexList()
{
	return this->m_size_list;
}

bool VertexBuffer::release()
{
	m_layout->Release();
	m_buffer->Release();
	delete this;
	return true;
}

VertexBuffer::~VertexBuffer()
{

}

