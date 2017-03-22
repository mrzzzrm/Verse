#version 330

uniform sampler2D Texture;

in vec2 f_UV;

out vec4 o_Color;

void main()
{
    o_Color = texture(Texture, f_UV);
}