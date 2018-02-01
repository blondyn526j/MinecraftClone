#version 330

out vec4 color;
in vec2 position0;
uniform sampler2D renderMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

float range = 0.05;

	vec3 sampleSphere[16]=
	vec3[16](
		vec3( 0.5381, 0.1856,-0.4319), vec3( 0.1379, 0.2486, 0.4430),
		vec3( 0.3371, 0.5679,-0.0057), vec3(-0.6999,-0.0451,-0.0019),
		vec3( 0.0689,-0.1598,-0.8547), vec3( 0.0560, 0.0069,-0.1843),
		vec3(-0.0146, 0.1402, 0.0762), vec3( 0.0100,-0.1924,-0.0344),
		vec3(-0.3577,-0.5301,-0.4358), vec3(-0.3169, 0.1063, 0.0158),
		vec3( 0.0103,-0.5869, 0.0046), vec3(-0.0897,-0.4940, 0.3287),
		vec3( 0.7119,-0.0154,-0.0918), vec3(-0.0533, 0.0596,-0.5411),
		vec3( 0.0352,-0.0631, 0.5460), vec3(-0.4776, 0.2847,-0.0271));

float rand(vec2 co){
    return fract(sin(dot(co.xy,vec2(12.9898,78.233))) * 43758.5453)*0.5 - 1.0;
}
float rand2(vec2 co){
    return fract(sin(dot(co.xy,vec2(8.718,78.233))) * 1688.5453)*0.5 - 1.0;
}

float GetLinearDepth(vec2 pos)
{
	float f=320;
	float n = 0.01;
	return (2 * n) / (f + n - texture2D(depthMap, pos).r * (f - n));
}

float LinearDepth(float depthSample)
{
	float zNear = 0.01;
	float zFar = 8;
    depthSample = 2.0 * depthSample - 1.0;
    float zLinear = 2.0 * zNear / (zFar + zNear - depthSample * (zFar - zNear));
    return zLinear;
}

float GetSSAOComponent(vec2 offset, float currentDepth)
{
	float val = abs(GetLinearDepth(offset) - currentDepth);
	//if(val < range)
		return val;
	//else
	//	return 0;
}

vec3 GetNormal(float depth)
{
	vec2 offset1 = vec2(0.0, 0.001);
	vec2 offset2 = vec2(0.001, 0.0);
	float depth1 = LinearDepth(texture2D(depthMap, position0+offset1).r);
	float depth2 = LinearDepth(texture2D(depthMap, position0+offset2).r);
	vec3 p1 = vec3(offset1, depth1 - depth);
	vec3 p2 = vec3(offset2, depth2 - depth);

	vec3 normal = cross(p1, p2);
	normal.z = -normal.z;
	return normal;
}

float testSSAO()
{
	float strength = 0.9;
	float base = 0.05;

	float area = 0.95;
	float falloff = 0.001;
	float radius = 0.02;
	int samples = 16;

	vec3 random = normalize(texture2D(normalMap, 4.1*position0).rgb);
	float depth = LinearDepth(texture2D(depthMap, position0).r);
	vec3 position = vec3(position0.xy, depth);
	vec3 normal = GetNormal(depth);
	float radiusDepth = radius/depth;
	float occlusion = 0.0;

	for(int i = 0 ; i < samples; i++)
	{
		vec3 ray = radiusDepth * reflect(sampleSphere[i], random);
		vec3 hemiRay = position + sign(dot(ray, normal)) * ray;

		float occDepth = LinearDepth(texture2D(depthMap, clamp(hemiRay.xy, 0.0, 1.0)).r);
		float difference = depth - occDepth;

		occlusion += step(falloff, difference) * (1.0-smoothstep(falloff, area, difference));
	}
	float ao = strength * occlusion * (1.0 / samples);

	return ao;
}

void main() 
{
	//if(texture2D(depthMap, position0.xy).r >= 1.0)
	//	color = 1.0;
	//else
	//	color = mix(1.0, 0.0, testSSAO());

	//float ssaoComponent = 0;
	//float ssaoRange = 0.01;
	//float currentDepth = GetLinearDepth(vec2(0,0));
	//for(int i = 0; i < 32; ++i)
	//{
	//	float scale = float(i)/32.0;
	//	scale = mix(ssaoRange/20.0, ssaoRange, scale*scale);
	//	vec2 offsetVector = normalize(vec2(sin(6.283*float(i)/32.0), cos(6.283*float(i)/32.0)))*scale;
	//	ssaoComponent += GetSSAOComponent(offsetVector, currentDepth);
	//}
	//ssaoComponent *= 8.0;
	
	if(texture2D(depthMap, position0.xy).r >= 1.0)
	color = vec4(1, 1, 1, 1);
	else
	color = mix(vec4(1, 1, 1, 1), vec4(0, 0, 0, 1), testSSAO());
	//color = vec4(LinearDepth(texture2D(depthMap, position0.xy).r), 0, 0, 1);
	
	//color = testSSAO();texture2D(renderMap, position0.xy)
}