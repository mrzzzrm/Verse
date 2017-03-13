#version 330

uniform samplerCube Environment;
uniform vec3 CameraPosition;

in vec3 f_PositionWS;
in vec3 f_Normal;
in vec3 f_Color;

out vec3 o_Color;

void main()
{
    vec3 surfaceDir = normalize(f_PositionWS - CameraPosition);
    vec3 reflectDir = reflect(surfaceDir, f_Normal);

    vec3 light = normalize(vec3(0.1f, 1.0f, 0.2f));
	o_Color = f_Color * 0.5 + f_Color * dot(f_Normal, light) * 0.3f + texture(Environment, reflectDir).rgb * 0.2f;
}