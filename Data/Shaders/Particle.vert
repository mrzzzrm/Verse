#version 330

uniform Globals
{
    mat4 View;
    mat4 Projection;
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
in mat3 BirthOrientation;
in float BirthScale;
in float DeathScale;
in vec4 AxisRotation; // (x,y,z) - UnitAxis, w - speed

in vec3 Position;
in vec2 UV;
in vec3 Normal;
in vec4 Color;

out vec3 f_Normal;
out vec2 f_UV;
out vec4 f_RGBA;
out float f_NormalisedAge;
out vec3 f_PositionVS;

vec3 RodriguesRotation(vec3 p, vec4 axisAngle) {
  return (
    p * cos(axisAngle.w) + cross(axisAngle.xyz, p) *
    sin(axisAngle.w) + axisAngle.xyz * dot(p, axisAngle.xyz) *
    (1.0 - cos(axisAngle.w))
  );
}

void main()
{
    float ageSeconds = float(Time - Birth) / 1000.0f;
	f_NormalisedAge = min(float(Time - Birth) / Lifetime, 1.0f);

    if (f_NormalisedAge >= 1.0f) {
        gl_Position = vec4(0.0f);
        f_Normal = vec3(0.0f);
        return;
    }

    float scale = mix(BirthScale, DeathScale, f_NormalisedAge);

    vec3 particleCenter = Origin + (float(Time - Birth) / 1000.0f) * Velocity;
    vec3 vertexPosition;

    vec4 axisAngle = AxisRotation;
    axisAngle.w *= ageSeconds;

    vec3 scaledRotatedPosition = RodriguesRotation(Position, axisAngle) * scale;

    if (OrientationType == 0) // World
    {
        vertexPosition = BirthOrientation * scaledRotatedPosition + particleCenter;
    }
    else if (OrientationType == 1) // ViewBillboard
    {
        vertexPosition = (ViewBillboardRotation * scaledRotatedPosition) + particleCenter;
    }

    vec3 positionVS = (View * vec4(vertexPosition, 1.0f)).xyz;
	vec4 transformed = Projection * vec4(positionVS, 1.0f);

	gl_Position = transformed;
	f_Normal = Normal;
	f_RGBA = mix(BirthRGBA, DeathRGBA, f_NormalisedAge) * Color;
	f_UV = UV;
	f_PositionVS = positionVS;
}

