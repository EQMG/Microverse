#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 0) uniform UboScene 
{
	vec4 colour;
	float radius;
	float softness;
} scene;

layout(rgba16f, set = 0, binding = 1) uniform writeonly image2D writeColour;

layout(set = 0, binding = 2) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUv;

layout(location = 0) out vec4 outColour;

void main() 
{
	vec3 colour = texture(samplerColour, inUv).rgb;

	float len = 1.0f - length(inUv - vec2(0.5f));
	float vignette = smoothstep(scene.radius, scene.radius - scene.softness, len);

	outColour = vec4(colour + (scene.colour.a * (vignette * scene.colour.rgb)), 1.0);

	vec2 sizeColour = textureSize(samplerColour, 0);
	imageStore(writeColour, ivec2(inUv * sizeColour), outColour);
}
