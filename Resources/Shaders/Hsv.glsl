const vec3 kRGBToYPrime = vec3(0.299f, 0.587f, 0.114f);
const vec3 kRGBToI = vec3(0.596f, -0.275f, -0.321f);
const vec3 kRGBToQ = vec3(0.212f, -0.523f, 0.311f);

const vec3 kYIQToR = vec3(1.0f, 0.956f, 0.621f);
const vec3 kYIQToG = vec3(1.0f, -0.272f, -0.647f);
const vec3 kYIQToB = vec3(1.0f, -1.107f, 1.704f);

vec3 rgbHue(vec3 source, float adjust)
{
	// Convert to YIQ.
	float YPrime = dot(source, kRGBToYPrime);
	float I = dot(source, kRGBToI);
	float Q = dot(source, kRGBToQ);

	// Calculate the hue and chroma
	float hue = atan(Q, I);
	float chroma = sqrt(I * I + Q * Q);

	// Make the adjustments
	hue += adjust;

	// Convert back to YIQ
	Q = chroma * sin(hue);
	I = chroma * cos(hue);

	// Convert back to RGB
	vec3 yIQ = vec3(YPrime, I, Q);
	return vec3(dot(yIQ, kYIQToR), dot(yIQ, kYIQToG), dot(yIQ, kYIQToB));
}

vec3 rgb2hsv(vec3 c)
{
	vec4 K = vec4(0.0f, -1.0f / 3.0f, 2.0f / 3.0f, -1.0f);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);
	float e = 1.0e-10f;
	return vec3(abs(q.z + (q.w - q.y) / (6.0f * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
	vec4 K = vec4(1.0f, 2.0f / 3.0f, 1.0f / 3.0f, 3.0f);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0f - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0f, 1.0f), c.y);
}
