#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(push_constant) uniform PushScene
{
	vec4 colour;
	float radius;
	float softness;
} scene;

layout(binding = 0, rgba8) uniform writeonly image2D writeColour;

layout(binding = 1) uniform sampler2D samplerColour;

layout(location = 0) in vec2 inUV;

void main() 
{
	vec4 colour = texture(samplerColour, inUV);

	float len = 1.0f - length(inUV - vec2(0.5f));
	float vignette = smoothstep(scene.radius, scene.radius - scene.softness, len);

	colour = vec4(colour.rgb + (scene.colour.a * (vignette * scene.colour.rgb)), 1.0);
	
	imageStore(writeColour, ivec2(inUV * imageSize(writeColour)), colour);
}
