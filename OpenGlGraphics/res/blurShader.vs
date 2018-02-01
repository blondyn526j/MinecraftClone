#version 330
attribute vec3 position;
uniform mat4 transform;
out vec2 position0;
void main() 
{
	position0 = (position.xy + vec2(1, 1))*0.5;
	//position0.y = 1-position0.y;
	gl_Position = vec4(position.xy, -1.0, 1.0);
}