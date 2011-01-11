
#version 150

uniform mat4 projMat;
uniform mat4 mvMat;
uniform mat4 normMat;
uniform float vScale;

in vec3 CellularPos;
in vec3 CellularVel;

out vec3 fNormal;
out vec3 fViewDir;
out vec3 fLightDir;

void main(void)
{		
	gl_Position = projMat * mvMat * vec4(vec3(CellularPos.x, CellularPos.z, CellularPos.y)*vScale, 1.0);
	
	gNormal = normalize(normMat*vec4(1.0,1.0,1.0,1.0) + vec4(vec3(CellularVel.x, CellularVel.z, CellularVel.y), 1.0)).xyz;
	vec4 pos = mvMat * vec4(vec3(CellularPos.x, CellularPos.z, CellularPos.y)*vScale, 1.0);
	gViewDir = -(pos.xyz / pos.w);
	gLightDir = vec3((mvMat * vec4(0.0, 0.0, 0.0, 1.0))- pos);
}
