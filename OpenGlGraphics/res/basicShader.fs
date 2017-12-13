#version 330

varying vec2 texCoord0;
flat in int normal0;
in float timer0;
in vec3 position0;
in vec3 camPosition0;
vec3 normal;
uniform sampler2D diffuse;

void main()
{
	vec3 lightDir = normalize(vec3(0.3, 1, 0.1));
	//if(normal0 == 0)
	//	normal = vec3(0, 1, 0);
	//else if(normal0 == 1)
	//	normal = vec3(0, 0, 1);
	//else if(normal0 == 3)
	//	normal = vec3(-1, 0, 0);
	//else
	if(normal0 == 0)
	normal = vec3(0,1,0);
	else if(normal0 == 1)
	normal = vec3(0,0,1);
	else if(normal0 == 2)
	normal = vec3(0,-1,0);
	else if(normal0 == 3)
	normal = vec3(1,0,0);
	else if(normal0 == 4)
	normal = vec3(0,0,-1);
	else if(normal0 == 5)
	normal = vec3(-1,0,0);

	//vec4 tex = vec4(sin(position0.x), sin(position0.y), sin(position0.z), 1);
	vec4 tex = texture2D(diffuse, texCoord0);
	vec4 specular;
	if(tex.a < 0.9)
	discard;
	else
	{
	tex = vec4( tex.xyz * (0.65 + 0.35 * 
	clamp(
	dot(
	normal, lightDir),
	-1,1)), 1.0);
	vec3 fragmentToSurface = normalize(camPosition0 - position0);
	specular = vec4(vec3(0.1, 0.1, 0.1) * max(0.0, pow(dot(lightDir, fragmentToSurface), 5)), 1.0);
	tex += specular;
	gl_FragColor = tex; 
	}

}