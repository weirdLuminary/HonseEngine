#include "Query.h"

Query::Query(GLenum target) : m_Type(target) {
    glGenQueries(1, &m_RendererID);
}

Query::~Query() {
    if(m_RendererID != 0) glDeleteQueries(1, &m_RendererID);
}

void Query::Begin() {
    glBeginQuery(m_Type, m_RendererID);
}

void Query::End() {
    glEndQuery(m_Type);
}

const GLint64 Query::GetResult() {
    GLint64 res;
    glGetQueryObjecti64v(m_RendererID, GL_QUERY_RESULT, &res);
    return res;
}