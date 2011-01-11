
#version 140

uniform mat4 projMat;
uniform mat4 mvMat;
uniform mat4 normMat;
uniform float vScale;

in float ParticleIndex;
in float ParticleSize;
in vec3 ParticlePos;
in vec3 ParticleVel;

out vec3 fNormal;
out vec3 fViewDir;
out vec3 fLightDir;
out float fParticleIndex;
out float fParticleSize;

void main(void)
{	
	gl_Position = projMat * mvMat * vec4(vec3(ParticlePos.x, ParticlePos.z, ParticlePos.y)*vScale, 1.0);
	
	fNormal = normalize(normMat*vec4(1.0,1.0,1.0,1.0) + vec4(vec3(ParticleVel.x, ParticleVel.z, ParticleVel.y), 1.0)).xyz;
	vec4 pos = mvMat * vec4(ParticlePos, 1.0);
	fViewDir = -(pos.xyz / pos.w);
	fLightDir = vec3((mvMat * vec4(0.0, 0.0, 0.0, 1.0))- pos);
	
	fParticleIndex = ParticleIndex;
	fParticleSize = ParticleSize;
}
