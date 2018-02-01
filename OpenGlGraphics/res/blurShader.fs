#version 330

out vec4 color;
in vec2 position0;
uniform sampler2D renderMap;
uniform sampler2D ssaoMap;
uniform sampler2D depthMap;

void main() 
{
	float ssaoComponent = 0.0;
	int range = 3;
	float step = 0.002;
	float ignoreThreshold = 0.00006;
	float zCurrent = texture2D(depthMap, position0.xy).r;
	float zDiffrence;
	int numSamples = 0;
	for(int x = -range; x <= range; x++)
	{
		for(int y = -range; y <= range; y++)
		{
			zDiffrence = abs(texture2D(depthMap, position0.xy + vec2(float(x)*step, float(y)*step)).r - zCurrent);
			if(zDiffrence < ignoreThreshold)
			{
				ssaoComponent+=texture2D(ssaoMap, position0.xy + vec2(float(x)*step, float(y)*step)).r;
				numSamples++;
			}
		}
	}
	ssaoComponent/=float(numSamples);
	color = mix(vec4(0,0,0,1), texture2D(renderMap, position0.xy), ssaoComponent);
	//texture2D(renderMap, position0.xy)
}