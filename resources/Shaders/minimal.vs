
#version 140

uniform mat4 projMat;
uniform mat4 mvMat;

in vec3 vVertex;

void main(void)
{	
	gl_Position = projMat * mvMat * vec4(vVertex, 1.0);
}
