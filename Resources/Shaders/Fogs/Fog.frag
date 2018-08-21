#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform UboObject
{
	mat4 transform;
	vec3 cameraPos;
	vec3 lightDir;
	vec3 invWavelength;
	float cameraHeight;
	float innerRadius;
	float outerRadius;
	float scaleDepth;
} object;

layout(location = 0) in vec3 inPosition;

layout(location = 0) out vec4 outColour;

const float pi = 3.1415926535897932384626433832795f;
const float g = -0.99f;
const float g2 = g*g;
const float fKrESun = 0.0025f * 15.0f;
const float fKmESun = 0.0015f * 15.0f;
const float fKr4PI = 0.0025f * 4.0f * pi;
const float fKm4PI = 0.0015f * 4.0f * pi;

float sqr(float x)
{
	return x * x;
}

float getNearIntersection(vec3 pos, vec3 ray, float distance2, float radius2)
{
	float B = 2.0f * dot(pos, ray);
	float C = distance2 - radius2;
	float det = max(0.0f, sqr(B) - 4.0f * C);
	return 0.5f * (-B - sqrt(det));
}

float scale(float fCos)
{
    float x = 1.0f - fCos;
    return object.scaleDepth * exp(-0.00287f + x * (0.459f + x * (3.83f + x * (-6.80f + x * 5.25f))));
}

void main()
{
	float fSamples = 3.0f;
	float fScale = 1.0f / (object.outerRadius - object.innerRadius);
	float fScaleOverScaleDepth = fScale / object.scaleDepth;

	// Get the ray from the camera to the vertex and its length (which
	// is the far point of the ray passing through the atmosphere)
	vec3 v3Ray = inPosition - object.cameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the ray's start and end positions in the atmosphere,
	// then calculate its scattering offset
    float fStartOffset;
    vec3 v3Start;

	// Calculate the closest intersection of the ray with
	// the outer atmosphere (point A in Figure 16-3)
    if (length(object.cameraPos) > object.outerRadius) {
        float fNear = getNearIntersection(object.cameraPos, v3Ray, sqr(object.cameraHeight), sqr(object.outerRadius));
        v3Start = object.cameraPos + v3Ray * fNear;
        fFar -= fNear;
        float fStartAngle = dot(v3Ray, v3Start) / object.outerRadius;
        float fStartDepth = exp(-1.0 / object.scaleDepth);
        fStartOffset = fStartDepth * scale(fStartAngle);
    } else {
        v3Start = object.cameraPos;
        float fHeight = length(v3Start);
        float fDepth = exp(fScaleOverScaleDepth * (object.innerRadius - object.cameraHeight));
        float fStartAngle = dot(v3Ray, v3Start) / fHeight;
        fStartOffset = fDepth*scale(fStartAngle);
    }

	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5f;

	// Now loop through the sample points
	vec3 v3FrontColor = vec3(0.0f);

	for(int i=0; i<fSamples; i++)
	{
        float fHeight = length(v3SamplePoint);
        float fDepth = exp(fScaleOverScaleDepth * (object.innerRadius - fHeight));
        float fLightAngle = dot(object.lightDir, v3SamplePoint) / fHeight;
        float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
        float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
        vec3 v3Attenuate = exp(-fScatter * (object.invWavelength * fKr4PI + fKm4PI));
        v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
        v3SamplePoint += v3SampleRay;
    }

    // Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
    vec4 secondaryColor = vec4(v3FrontColor * fKmESun, 1.0);
    vec4 primaryColor = vec4(v3FrontColor * (object.invWavelength * fKrESun), 1.0);
    vec3 v3Direction = object.cameraPos - inPosition;

    float fCos = dot(object.lightDir, v3Direction) / length(v3Direction);
    float fRayleighPhase = 0.75 * (1.0 + fCos*fCos);
    float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos*fCos) / pow(1.0 + g2 - 2.0*g*fCos, 1.5);
    outColour = fRayleighPhase * primaryColor + fMiePhase * secondaryColor;
}
