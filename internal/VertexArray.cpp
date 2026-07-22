#include "VertexArray.h"

VertexArray::VertexArray() {

    glGenVertexArrays(1, &m_RendererID);

}

void VertexArray::Init() {
    
    glGenVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const { glBindVertexArray(m_RendererID); }
void VertexArray::Unbind() { glBindVertexArray(0); }

void VertexArray::AddBuffer(const VertexBuffer& vb, const std::vector<VertexAttribute>& layout) 
{

    Bind();
    vb.Bind();

    unsigned int stride = 0;

    for(auto& element : layout) 
        stride += element.count * VertexAttribute::GetSize(element.type);


    unsigned int offset = 0;

    for (unsigned int i = 0; i < layout.size(); ++i) {

        const auto& element = layout[i];

        glEnableVertexAttribArray(element.location);

        if (element.type == GL_INT) {
            glVertexAttribIPointer(element.location, element.count, element.type, stride, reinterpret_cast<const void*>(offset));
        }
        else
        {
            glVertexAttribPointer(element.location, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<const void*>(offset));
        }
        glVertexAttribDivisor(element.location, 0);

        offset += element.count * VertexAttribute::GetSize(element.type);
    }

}
