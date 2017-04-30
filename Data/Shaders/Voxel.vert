#version 330

uniform mat4 View;
uniform mat4 Projection;
uniform float Scale;
uniform mat4 Transform;

in vec3 Position;
in vec3 Normal;
in vec3 Color;

out vec3 f_PositionVS;
out vec3 f_Normal;
out vec3 f_Color;

void main()
{
    vec4 positionVS = View * Transform * vec4(Scale * Position, 1.0f);

    gl_Position = Projection * positionVS;

    f_PositionVS = positionVS.xyz;
	f_Normal = (View * Transform * vec4(Normal, 0.0f)).xyz;
    f_Color = Color;
}