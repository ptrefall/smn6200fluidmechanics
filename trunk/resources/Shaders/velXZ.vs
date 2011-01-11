
#version 150

uniform mat4 projMat;
uniform mat4 mvMat;
uniform mat4 normMat;
uniform float vScale;

in vec3 CellularPos;
in vec3 CellularVel;

out float gScale;
out vec4 gVelocity;
out vec3 gNormal;
out vec3 gViewDir;
out vec3 gLightDir;

void main(void)
{				
	gl_Position = vec4(vec3(CellularPos.x, CellularPos.z, CellularPos.y)*vScale, 1.0);
	
	gScale = vScale;
	gVelocity = vec4(vec3(CellularVel.x, CellularVel.z, CellularVel.y), 0.0);
	
	gNormal = normalize(normMat*vec4(1.0,1.0,1.0,1.0) + vec4(vec3(CellularVel.x, CellularVel.z, CellularVel.y), 1.0)).xyz;
	vec4 pos = mvMat * vec4(vec3(CellularPos.x, CellularPos.z, CellularPos.y)*vScale, 1.0);
	gViewDir = -(pos.xyz / pos.w);
	gLightDir = vec3((mvMat * vec4(0.0, 0.0, 0.0, 1.0))- pos);
}
