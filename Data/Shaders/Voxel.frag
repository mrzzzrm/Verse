#version 330

uniform samplerCube Environment;

in vec3 f_PositionVS;
in vec3 f_Normal;
in vec3 f_Color;

out vec3 o_Color;
out vec3 o_Position;
out vec3 o_Normal;

void main()
{
    vec3 surfaceDir = normalize(f_PositionVS);
    vec3 reflectDir = reflect(surfaceDir, f_Normal);

    vec3 light = normalize(vec3(0.1f, 1.0f, 0.2f));
	o_Color = f_Color * 0.5 + f_Color * dot(f_Normal, light) * 0.3f + texture(Environment, reflectDir).rgb * 0.2f;
    o_Position = f_PositionVS;
    o_Normal = f_Normal;
}