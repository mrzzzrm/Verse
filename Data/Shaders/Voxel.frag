#version 330

uniform samplerCube Environment;
uniform samplerBuffer Palette;

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

    vec3 color = texelFetch(Palette, int(f_ColorIndex)).rgb;

    vec3 light = normalize(vec3(0.1f, 1.0f, 0.2f));
	o_Diffuse = color * 0.5 + color * dot(f_Normal, light) * 0.3f + texture(Environment, reflectDir).rgb * 0.2f;
    o_Position = f_PositionVS;
    o_Normal = f_Normal;
}