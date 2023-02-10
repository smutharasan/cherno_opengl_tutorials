#pragma once

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray(const void * data, unsigned int size);
	~VertexArray();

	void Bind() const;
	void UnBind() const;

};