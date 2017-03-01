#version 330

uniform Globals
{
    mat4 ViewProjection;
    uint Time;
};

uniform int OrientationType;
uniform mat3 ViewBillboardRotation;

in vec3 Origin;
in vec3 Velocity;
in uint Birth;
in uint Lifetime;
in vec4 BirthRGBA;
in vec4 DeathRGBA;
in vec4 BirthOrientation;
in float BirthScale;
in float DeathScale;

in vec3 Position;
in vec2 UV;
in vec3 Normal;

out vec3 f_Normal;
out vec2 f_UV;
out vec4 f_RGBA;
out float f_NormalisedAge;

void main()
{
	f_NormalisedAge = min(float(Time - Birth) / Lifetime, 1.0f);

    if (f_NormalisedAge >= 1.0f) {
        gl_Position = vec4(0.0f);
        f_Normal = vec3(0.0f);
        return;
    }

    float scale = mix(BirthScale, DeathScale, f_NormalisedAge);

    vec3 particleCenter = Origin + (float(Time - Birth) / 1000.0f) * Velocity;
    vec3 vertexPosition;
    vec3 scaledPosition = Position * scale;

    if (OrientationType == 0) // World
    {
        vertexPosition = BirthOrientation.xyz * 0.01f + scaledPosition + particleCenter;
    }
    else if (OrientationType == 1) // ViewBillboard
    {
        vertexPosition = (ViewBillboardRotation * scaledPosition) + particleCenter;
    }

	vec4 transformed = ViewProjection * vec4(vertexPosition, 1.0f);

	gl_Position = transformed;
	f_Normal = Normal;
	f_RGBA = mix(BirthRGBA, DeathRGBA, f_NormalisedAge);
	f_UV = UV;
}

