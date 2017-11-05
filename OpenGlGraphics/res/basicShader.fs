#version 120

varying vec2 texCoord0;

uniform sampler2D diffuse;

void main()
{
	vec4 tex = texture2D(diffuse, texCoord0);
	// tex.r += 0.1;
	gl_FragColor = tex;
	//gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}
