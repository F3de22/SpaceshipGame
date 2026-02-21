#version 400

layout (location = 0) in vec2 aPos;

out vec2 TexCoord;

uniform vec2 res;

void main()
{
    TexCoord = (0.5 + aPos * 0.5); 
    gl_Position = vec4(aPos, 0., 1.);
}