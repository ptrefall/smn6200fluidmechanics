
#version 150

uniform mat4 projMat;
uniform mat4 mvMat;
uniform mat4 normMat;
uniform float vScale;
uniform float vShowInHalf;

in float ParticleIndex;
in float ParticleSize;
in vec3 ParticlePos;
in vec3 ParticleVel;

out float gScale;
out vec4 gVelocity;
out vec3 gNormal;
out vec3 gViewDir;
out vec3 gLightDir;
out float gParticleIndex;
out float gParticleSize;

void main(void)
{	
	if(vShowInHalf != 0.0)
		if(ParticlePos.y < 0.0)
			discard;
			
	gl_Position = vec4(vec3(ParticlePos.x, ParticlePos.z, ParticlePos.y)*vScale, 1.0);
	
	gScale = vScale;
	gVelocity = vec4(vec3(ParticleVel.x, ParticleVel.z, ParticleVel.y), 0.0);
	
	gNormal = normalize(normMat*vec4(1.0,1.0,1.0,1.0) + vec4(vec3(ParticleVel.x, ParticleVel.z, ParticleVel.y), 1.0)).xyz;
	vec4 pos = mvMat * vec4(vec3(ParticlePos.x, ParticlePos.z, ParticlePos.y)*vScale, 1.0);
	gViewDir = -(pos.xyz / pos.w);
	gLightDir = vec3((mvMat * vec4(0.0, 0.0, 0.0, 1.0))- pos);
	
	gParticleIndex = ParticleIndex;
	gParticleSize = ParticleSize;
}
