#version 330

varying vec2 texCoord0;
in vec3 position0;
in float timer0;
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

void main()
{
	vec3 lightDir = normalize(vec3(0.3, 1, 0.1));
	vec3 normal = normalize(cross( vec3(1, cos(timer0 + 0.8 * position0.x), 0), vec3(0, 0, -1))) +
	normalize(cross( vec3(0, sin(timer0 + 0.6 * position0.z), 1), vec3(1, 0, 0)));
	normal = normalize(normal);
	vec4 tex = texture2D(diffuseMap, texCoord0);
	tex = vec4( tex.xyz * (0.55 + 0.55 * 
	clamp(
	dot(
	normal, lightDir),
	-1,1)), 0.85);
	//tex.a = 0.7;
	gl_FragColor = tex;
	//gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}
