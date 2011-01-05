
#version 140

uniform mat4 projMat;
uniform mat4 mvMat;

in vec2 vVertex;
in vec4 vColor;
in vec2	vTexCoord;

out vec4 fColor;
smooth out vec2 fTexCoord;

void main(void)
{	
	fColor = vColor;
	fTexCoord = vTexCoord;
	gl_Position = projMat * mvMat * vec4(vVertex, 0.0, 1.0);
}
