#version 120

varying vec2 texCoord0;

attribute vec3 position;
attribute vec2 texCoord;

uniform float timer;
uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(position + vec3(0.0, -0.15 + 0.1*sin(timer + 0.17 * position.x + 0.21 * position.z), 0.0), 1.0);
	texCoord0 = texCoord;
}
