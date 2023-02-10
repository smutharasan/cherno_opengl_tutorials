#pragma once

#include "VertexBuffer.h"
#include "BufferLayout.h"

/*
* Figuratively this should represent a geomentry figure because this is a list of vertices. 
*
*/
class VertexArray 
{
private:
    unsigned int m_RendererID;
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

    void Bind() const;
    void Unbind() const;
};