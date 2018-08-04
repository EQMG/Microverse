#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	float innerRadius;
	float outerRadius;
} object;

layout(set = 0, binding = 2) uniform sampler2D samplerRingLookup;

layout(location = 0) in vec3 fragmentPosition;
layout(location = 1) in vec3 fragmentNormal;

layout(location = 0) out vec4 outColour;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec4 outMaterial;

#include "Shaders/Pipeline.glsl"
#include "Shaders/Noise.glsl"

void main()
{
	vec3 unitNormal = normalize(fragmentNormal);

    float len = length(fragmentPosition);

    if (len < object.innerRadius || len > object.outerRadius) // TODO: Remove this!
    {
        discard;
    }

    float u = (len - object.innerRadius) / (object.outerRadius - object.innerRadius);

    //if (abs(u) > 1.0f)
    //{
    //    discard;
    //}

    vec4 ringColour = texture(samplerRingLookup, vec2(u, 0.0f)).rgba;

	outColour = ringColour;
	outNormal = encodeNormal(unitNormal);
	outMaterial = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
