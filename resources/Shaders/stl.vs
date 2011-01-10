
#version 140

uniform mat4 projMat;
uniform mat4 mvMat;
uniform mat4 normMat;

in vec3 vVertex;
in vec3 vNormal;

out vec3 fNormal;
out vec3 fViewDir;
out vec3 fLightDir;

void main(void)
{	
	gl_Position = projMat * mvMat * vec4(vVertex, 1.0);
	
	fNormal = normalize(normMat*vec4(1.0,1.0,1.0,1.0) + vec4(vNormal, 1.0)).xyz;
	vec4 pos = mvMat * vec4(vVertex, 1.0);
	fViewDir = -(pos.xyz / pos.w);
	fLightDir = vec3((mvMat * vec4(0.0, 0.0, 0.0, 1.0))- pos);
}
