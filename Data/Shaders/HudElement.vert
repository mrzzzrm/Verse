#version 330

uniform sampler2D Texture;
uniform vec2 ViewportSize;

in vec2 ElementPosition;
in vec2 Flip;
in vec2 Position;
in vec2 UV;

out vec2 f_UV;

void main()
{
    vec2 scale = textureSize(Texture, 0) / ViewportSize;

    gl_Position = vec4(ElementPosition + (Position * Flip * scale), 0.0f, 1.0f);
    f_UV = UV;
}