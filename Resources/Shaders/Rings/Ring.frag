#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	float innerRadius;
	float outerRadius;
} object;

layout(set = 0, binding = 2) uniform sampler2D samplerRingLookup;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 outColour;

void main()
{
	float len = length(inPosition);

	if (len < object.innerRadius || len > object.outerRadius)
	{
		discard;
	}

	float u = (len - object.innerRadius) / (object.outerRadius - object.innerRadius);

	outColour = texture(samplerRingLookup, vec2(u, 0.0f));
}
