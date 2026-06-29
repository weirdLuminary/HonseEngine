#include "VertexArray.h"

VertexArray::VertexArray() {

    glGenVertexArrays(1, &m_RendererID);

}

void VertexArray::Bind() const { glBindVertexArray(m_RendererID); }
void VertexArray::Unbind() const { glBindVertexArray(0); }

void VertexArray::AddBuffer(const VertexBuffer& vb, const std::vector<VertexAttribute>& layout) 
{

    Bind();
    vb.Bind();

    unsigned int stride = 0;

    for(auto& element : layout) 
        stride += element.count * VertexAttribute::GetSize(element.type);


    unsigned int offset = 0;

    for(unsigned int i = 0; i < layout.size(); ++i) {

        const auto& element = layout[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, element.count, element.type, element.normalized ? GL_TRUE : GL_FALSE, stride, reinterpret_cast<const void*>(offset));
        offset += element.count * VertexAttribute::GetSize(element.type);

    }

}
