
#version 140

uniform samplerCube fCube;
smooth in vec3 fTexCoord;

out vec4 out_Color0;

void main()
{
	vec4 texColor = texture(fCube, fTexCoord);
	out_Color0 = vec4(texColor.rgb, 1.0);
}
