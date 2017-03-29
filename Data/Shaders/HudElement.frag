#version 330

uniform sampler2D Texture;

in vec2 f_UV;
in vec3 f_Color;

out vec4 o_Color;

void main()
{
    o_Color = texture(Texture, f_UV) * vec4(f_Color, 1.0f);
    gl_FragDepth = 0.0f;
}