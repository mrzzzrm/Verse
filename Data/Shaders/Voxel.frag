#version 330

uniform samplerCube Environment;
uniform samplerBuffer ColorPalette;
uniform samplerBuffer BrightnessScalePalette;

in vec3 f_PositionVS;
in vec3 f_Normal;
flat in uint f_ColorIndex;

out vec3 o_Diffuse;
out vec3 o_Position;
out vec3 o_Normal;

void main()
{
    vec3 surfaceDir = normalize(f_PositionVS);
    vec3 reflectDir = reflect(surfaceDir, f_Normal);

    // TODO do in vertex shader?!
    vec3 color = texelFetch(ColorPalette, int(f_ColorIndex)).rgb;
    color *= texelFetch(BrightnessScalePalette, int(f_ColorIndex)).r;

    vec3 light = normalize(vec3(0.1f, 1.0f, 0.2f));
	o_Diffuse = color * 0.5 + color * dot(f_Normal, light) * 0.3f + texture(Environment, reflectDir).rgb * 0.2f;
    o_Position = f_PositionVS;
    o_Normal = f_Normal;
}