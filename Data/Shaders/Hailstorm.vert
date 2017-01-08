#version 330

uniform Globals
{
    mat4 ViewProjection;
    uint Time;
};

in vec3 Origin;
in vec3 Velocity;
in uint Birth;
in uint Lifetime;

in vec3 Position;
in vec3 Normal;

out vec3 f_Normal;
out float f_Alive;

void main()
{
    if (Time > Birth + Lifetime) {
        gl_Position = vec4(0.0f);
        f_Normal = vec3(0.0f);
        f_Alive = 0.0f;
        return;
    }

    vec3 position = Origin + Position + (float(Time - Birth) / 1000.0f) * Velocity;
	vec4 transformed = ViewProjection * vec4(position, 1.0f);

	gl_Position = transformed;
	f_Normal = (vec4(Normal, 0.0f)).xyz;
	f_Alive = 1.0f;
}

