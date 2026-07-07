#version 330 core

in vec2 v_TexCoord;
flat in int v_TextureID;
in vec4 v_Color;

out vec4 FragColor;

uniform sampler2D u_Textures[16];

void main()
{
    FragColor = texture(u_Textures[v_TextureID], v_TexCoord) * v_Color;
}