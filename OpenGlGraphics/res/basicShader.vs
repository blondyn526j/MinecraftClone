#version 130

varying vec2 texCoord0;
flat out int normal0;
out float timer0;
attribute vec3 position;
attribute vec2 texCoord;
attribute int normal;
uniform float timer;

uniform mat4 transform;

void main()
{
	normal0 = normal;
	timer0 = timer;
	gl_Position = transform * vec4(position, 1.0);
	texCoord0 = texCoord;
}
