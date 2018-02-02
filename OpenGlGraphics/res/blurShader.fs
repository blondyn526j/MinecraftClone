#version 330

out vec4 color;
in vec2 position0;
uniform sampler2D renderMap;
uniform sampler2D ssaoMap;
uniform sampler2D depthMap;

float LinearDepth(float depthSample)
{
	float zNear = 0.01;
	float zFar = 8;
    depthSample = 2.0 * depthSample - 1.0;
    float zLinear = 2.0 * zNear / (zFar + zNear - depthSample * (zFar - zNear));
    return zLinear;
}

void main() 
{
	float ssaoComponent = 0.0;
	int range = 3;
	float step = 0.0006;
	float ignoreThreshold = 0.01;
	float zCurrent = LinearDepth(texture2D(depthMap, position0.xy).r);
	float zDiffrence;
	float totalWeight = 0;
	for(int x = -range; x <= range; x++)
	{
		for(int y = -range; y <= range; y++)
		{
			vec2 offset = vec2(float(x)*step, float(y)*step)/zCurrent;
			zDiffrence = abs(LinearDepth(texture2D(depthMap, position0.xy +offset).r) - zCurrent);
			if(zDiffrence < ignoreThreshold)
			{
				float weight = 1.0 - 0.01*(abs(float(x))/range + abs(float(y))/range);
				//float weight = 1.0;
				ssaoComponent+=texture2D(ssaoMap, position0.xy + offset).r * weight;
				totalWeight += weight;
			}
		}
	}
	ssaoComponent/=totalWeight;
	color = mix(vec4(0,0,0,1), texture2D(renderMap, position0.xy),ssaoComponent);
	//color = mix(vec4(0,0,0,1), vec4(1,1,1,1), texture2D(ssaoMap, position0.xy).r);  texture2D(renderMap, position0.xy)

}