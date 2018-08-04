#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	float hueOffset;
	float time;
	int octaves;
} object;

layout(set = 0, binding = 2) uniform sampler2D samplerBandLookup;

layout(location = 0) in vec3 fragmentNormal;
layout(location = 1) in vec2 fragmentUv;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec4 outMaterial;

#include "Shaders/Pipeline.glsl"
#include "Shaders/Noise.glsl"
#include "Shaders/Hsv.glsl"

void main() 
{
	vec3 unitNormal = normalize(fragmentNormal);

    float xx = cos(unitNormal.y * 50.0f);
    vec3 position = unitNormal + xx * 0.04f;
    vec3 tOffset = vec3(object.time, 0.0f, object.time);
    position.xz *= 0.5f;
    position = position + tOffset;

    float n1 = noise(position, object.octaves + 1, 10.0f, 0.8f) * 0.01f;
    float n2 = ridgedNoise(position, object.octaves, 5.8f, 0.75f) * 0.015f - 0.01f;

    float s = 0.6;
    float t1 = snoise(position * 2.0f) - s;
    float t2 = snoise((position + 800.0f) * 2.0f) - s;
    float t3 = snoise((position + 1600.0f) * 2.0f) - s;
	float n3 = snoise(position * 0.1f) * max(t1 * t2 * t3, 0.0f);
	float final = (n1 + n2 + n3);

    float lookupCoord = fragmentUv.y + final;
	vec3 planetColour = texture(samplerBandLookup, vec2(0.5f, 2.0f * lookupCoord)).rgb;
	planetColour = rgbHue(planetColour, object.hueOffset);

	outColour = vec4(planetColour, 1.0f);
	outNormal = encodeNormal(unitNormal);
	outMaterial = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
