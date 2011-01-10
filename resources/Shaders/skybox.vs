
#version 140

uniform mat4 projMat;
uniform mat4 mvMat;

in vec3 vVertex;
in vec3 vTexCoord;

smooth out vec3 fTexCoord;

void main(void)
{
    fTexCoord = vTexCoord;
	gl_Position = projMat * mvMat * vec4(vVertex, 1.0);
}
