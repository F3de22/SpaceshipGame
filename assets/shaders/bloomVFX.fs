#version 400

out vec4 FragColor;
in vec2 TexCoord;

//textures
uniform sampler2D BlurTex;
uniform bool horizontal;

// Weights and offsets for the Gaussian blur
//uniform float Weight[10];
uniform float Weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);


void main()
{
    vec2 tex_offset = 1.0 / textureSize(BlurTex, 0); // gets size of single texel
    vec3 result = texture(BlurTex, TexCoord).rgb * Weight[0]; // current fragment's contribution
    
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(BlurTex, TexCoord + vec2(tex_offset.x * i, 0.0)).rgb * Weight[i];
            result += texture(BlurTex, TexCoord - vec2(tex_offset.x * i, 0.0)).rgb * Weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(BlurTex, TexCoord + vec2(0.0, tex_offset.y * i)).rgb * Weight[i];
            result += texture(BlurTex, TexCoord - vec2(0.0, tex_offset.y * i)).rgb * Weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}