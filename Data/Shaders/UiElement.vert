#version 330

uniform sampler2D Texture;
uniform vec2 ViewportSize;
uniform vec2 ElementPosition;

in vec2 Position;
in vec2 UV;

out vec2 f_UV;

void main()
{
    vec2 scale = textureSize(Texture, 0) / ViewportSize;

    gl_Position = vec4(ElementPosition + (Position * scale), 0.0f, 1.0f);
    f_UV = UV;
}