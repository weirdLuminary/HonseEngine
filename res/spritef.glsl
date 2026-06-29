#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform sampler2D glowTex;

uniform vec2 resolution;
uniform vec3 spriteColor;

void main()
{
    float t = gl_FragCoord.y / 640.0;
    t = pow(t, 2.0);   // slower rise
    vec4 glow = texture(glowTex, TexCoords)
              * t;

    color = glow + texture(image, TexCoords) * vec4(spriteColor, 1.0) / 2.0f;
}