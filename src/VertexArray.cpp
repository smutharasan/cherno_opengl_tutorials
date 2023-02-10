#include "VertexArray.h"
#include "BufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray(const void* givenData, unsigned int givenSize)
{
    GLCall(glGenBuffers(1, &m_RendererID)); // Generate a single buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // Select the buffer to be drawn
    GLCall(glBufferData(GL_ARRAY_BUFFER, givenSize, givenData, GL_STATIC_DRAW)); // Add the data to the buffer

}

VertexArray::~VertexArray()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexArray::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); // Select the buffer to be drawn
}

void VertexArray::UnBind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0)); // Unselect the buffer and reset it to 0
}
