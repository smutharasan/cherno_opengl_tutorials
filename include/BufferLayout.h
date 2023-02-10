#pragma once

#include <vector>
#include <GL\glew.h>

#include "Renderer.h"

/*
* Buffer Element is a specific vertex attribute property. For example, I would declare the position coordinates as a buffer element and specify the count as 2 or 3 if its 2D or 3D respectfully.
*/


struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:          return 4;
        case GL_UNSIGNED_INT:   return 4;
        case GL_UNSIGNED_BYTE:  return 1;
        }

        ASSERT(false);
        return 0;
    }
};

/*
* Vertex Buffer Layout contains a list of buffer elements, so a list of attributes
*  ^^ Represents a vertex essentially
    Example <position, texture, normal>
    Every time you want to add a new property use the push function
    You have mention the type that the values are going to take of.

    So lets say your adding the position property, then you would have to include that you're going to add float as the type because the values are provided are of float.

    Every time you add a property, the stride or 'size of the vertex' gets recalculated
*/

class VertexBufferLayout
{
private:
    std::vector<VertexBufferElement> m_Elements;
    unsigned int m_Stride;
public:
    VertexBufferLayout()
        : m_Stride(0) {}

    template<typename T>
    void Push(unsigned int count)
    {
        static_assert(sizeof(uintptr_t) == sizeof(void*), "Please provide the type you want the attribute to take shape of");

    }

    template<>
    void Push<float>(unsigned int count)
    {
        m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
    }

    template<>
    void Push<unsigned int>(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
    }

    template<>
    void Push<unsigned char>(unsigned int count)
    {
        m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_FALSE });
        m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
    }

    inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
    inline unsigned int GetStride() const { return m_Stride; }
};