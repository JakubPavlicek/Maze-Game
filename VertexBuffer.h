#pragma once

#include <GL/glew.h>

class VertexBuffer
{
private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size, GLenum draw);
    ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;
};
