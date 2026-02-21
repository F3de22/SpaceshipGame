float scan = 0.9; // simulate darkness between scanlines

layout (location = 0) out vec4 FragColor

in vec2 FragCoord;

uniform float time;
uniform vec2 res;


void main()
{
    // squared distance from center
    vec2 uv = fragCoord/res.xy;

    // determine if we are drawing in a scanline
    float apply = abs(sin(fragCoord.y + time * 2.) * 0.5 * scan);
    // sample the texture
    FragColor = vec4(mix(texture(iChannel0, uv).rgb, vec3(0.0), apply), 1.0);
}