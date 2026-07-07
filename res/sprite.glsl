#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in int  a_TextureID;
layout(location = 3) in vec4 a_Color;

out vec2 v_TexCoord;
flat out int v_TextureID;
out vec4 v_Color;

void main()
{
    gl_Position = a_Position;
    v_TexCoord = a_TexCoord;
    v_TextureID = a_TextureID;
    v_Color = a_Color;
}