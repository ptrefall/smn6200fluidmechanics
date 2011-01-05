
#version 140

uniform mat4 mvMat;

in vec2 vVertex;
in vec4 vColor;

out vec4 fColor;

void main(void)
{	
	fColor = vColor;
	gl_Position = mvMat * vec4(vVertex, 0.0, 1.0);
}
