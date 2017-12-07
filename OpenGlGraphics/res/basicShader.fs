#version 130

varying vec2 texCoord0;
flat in int normal0;
in float timer0;
vec3 normal;
uniform sampler2D diffuse;

void main()
{
	//if(normal0 == 0)
	//	normal = vec3(0, 1, 0);
	//else if(normal0 == 1)
	//	normal = vec3(0, 0, 1);
	//else if(normal0 == 3)
	//	normal = vec3(-1, 0, 0);
	//else
	if(normal0 == 1)
	normal = vec3(0,0,1);
	else if(normal0 == 0)
	normal = vec3(0,1,0);
	else if(normal0 == 3)
	normal = vec3(1,0,0);
	else if(normal0 == 4)
	normal = vec3(0,0,-1);
	else if(normal0 == 5)
	normal = vec3(-1,0,0);
	else
	normal = vec3(-1,1,0.5);

	vec4 tex = texture2D(diffuse, texCoord0);

	if(tex.a < 0.9)
	discard;
	else
	{
	//vec4 tex = vec4(1, 0, 0, 1);
	tex = vec4( tex.xyz * (0.65 + 0.35 * 
	clamp(
	dot(
	normal, normalize(vec3(0.3, 1, 0.1))),
	//normal, normalize(vec3(0.2, abs(cos(timer0/2.0)), 0.1))),
	-1,1)), 1.0);
	gl_FragColor = tex;
	//gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);	
	}

}
