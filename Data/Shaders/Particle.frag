#version 330

uniform sampler2D Texture;

in vec3 f_Normal;
in vec2 f_UV;
in float f_NormaliseAge;
in vec4 f_RGBA;

out vec4 o_Color;

void main()
{
    if (f_NormaliseAge >= 1.0f) discard;

//    vec3 light = normalize(vec3(0.1f, 1.0f, 0.2f));
//	o_Color = Color * 0.6 + Color * dot(f_Normal, light) * 0.4f;

    vec2 centeredUV =(f_UV * 2 - 1);
    float intensity = max(1.0f - length(centeredUV), 0.0f);

    vec4 tex = texture2D(Texture, f_UV);

    o_Color.rgb = tex.rgb * f_RGBA.rgb;
    o_Color.a = tex.a * f_RGBA.a * f_RGBA.a * tex.a;
}

