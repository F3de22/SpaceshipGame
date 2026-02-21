#version 400 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D albedo_tex;

//extract the bright information
float luma(vec3 color) 
{
    return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

void main()
{
    vec4 sample = texture(albedo_text, TexCoords);
    float brightness = luma(sample.rgb);

    if(brightness > 1)
        BrightColor = sample;
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);    
    FragColor = texture(albedo_tex, TexCoords);
}