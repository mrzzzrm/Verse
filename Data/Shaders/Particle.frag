#version 330

uniform bool DLightEnabled;
uniform vec3 DLightColor;
uniform vec3 DLightDirection;

uniform sampler2D Texture;

in vec3 f_Normal;
in vec3 f_PositionVS;
in vec2 f_UV;
in float f_NormalisedAge;
in vec4 f_RGBA;

out vec3 o_Normal;
out vec3 o_Position;
out vec4 o_Color;

void main()
{
    if (f_NormalisedAge >= 1.0f) discard;

    vec2 centeredUV =(f_UV * 2 - 1);
    float intensity = max(1.0f - length(centeredUV), 0.0f);

    vec4 tex = texture2D(Texture, f_UV);

    vec4 color = vec4(tex.rgb * f_RGBA.rgb, tex.a * f_RGBA.a);

    if (DLightEnabled)
    {
        color += color * dot(f_Normal, DLightDirection * DLightColor);
    }

    o_Color = color;
    o_Normal = f_Normal;
    o_Position = f_PositionVS;
}

