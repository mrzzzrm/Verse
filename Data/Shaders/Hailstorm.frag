#version 330

uniform vec3 Color;

in vec3 f_Normal;
in float f_Alive;

out vec3 o_Color;

void main()
{
    if (f_Alive == 0.0f)
    {
        discard;
    }

    vec3 light = normalize(vec3(0.1f, 1.0f, 0.2f));
	o_Color = Color * 0.6 + Color * dot(f_Normal, light) * 0.4f;
}

