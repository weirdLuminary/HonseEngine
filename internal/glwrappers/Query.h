#pragma once
#include <glad/glad.h>

class Query {

public:

    Query(GLenum type);
    Query() = default;
    ~Query();

    void Begin();
    void End();
    const GLint64 GetResult();

    Query(const Query&) = delete;
    Query& operator=(const Query&) = delete;

    Query(Query&& other) noexcept
    : m_RendererID(other.m_RendererID)
    {
        other.m_RendererID = 0;
    }

    Query& operator=(Query&& other) noexcept
    {
        if (this != &other)
        {
            if (m_RendererID != 0)
                glDeleteQueries(1, &m_RendererID);  

            m_RendererID = other.m_RendererID;
            m_Type = other.m_Type;
            other.m_Type = 0;
            other.m_RendererID = 0;
        }
        return *this;
    }

private:

    GLenum m_Type;
    unsigned int m_RendererID;

};