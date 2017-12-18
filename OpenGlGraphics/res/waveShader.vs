#version 330

varying vec2 texCoord0;
out vec3 position0;
out float timer0;

attribute vec3 position;
attribute vec2 texCoord;

uniform float timer;
uniform mat4 transform;

void main()
{
	position0 = position;
	timer0 = timer;
	gl_Position = transform * vec4(position + vec3(0.0, -0.3 + 0.15 * (sin(timer + 0.8 * position.x) + cos(timer + 0.6 * position.z)), 0.0), 1.0);
	texCoord0 = texCoord;
}