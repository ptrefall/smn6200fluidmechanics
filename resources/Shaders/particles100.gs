
#version 150
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
layout(line_strip, max_vertices = 2) out;

uniform mat4 projMat;
uniform mat4 mvMat;
uniform float vShowInHalf;

in float gScale[];
in vec4 gVelocity[];
in vec3 gNormal[];
in vec3 gViewDir[];
in vec3 gLightDir[];
in float gParticleIndex[];
in float gParticleSize[];

out float fDiscard;
out vec3 fNormal;
out vec3 fViewDir;
out vec3 fLightDir;
out float fParticleIndex;
out float fParticleSize;
smooth out vec4 fColor;

void main()
{
	int i;
	gl_Position = vec4(0.0);
	fDiscard = 0.0;
	fNormal = vec3(0.0);
	fViewDir = vec3(0.0);
	fLightDir = vec3(0.0);
	fParticleIndex = 0.0;
	fParticleSize = 0.0;
	fColor = vec4(0.0);

	//From point to line
	for(i=0; i< gl_in.length(); i++){
		if(vShowInHalf > 0.0)
		{
			if(gl_in[i].gl_Position.z < 0.0)
			{
				fDiscard = 1.0;
			}
		}
	
		gl_Position = projMat * mvMat * gl_in[i].gl_Position;
		fNormal = gNormal[i];
		fViewDir = gViewDir[i];
		fLightDir = gLightDir[i];
		fParticleIndex = gParticleIndex[i];
		fParticleSize = gParticleSize[i];
		fColor = vec4(0.0, 1.0, 0.0, 1.0);
		EmitVertex();
		
		gl_Position = projMat * mvMat * (gl_in[i].gl_Position + (gVelocity[i]*gScale[i]*0.1));
		fNormal = gNormal[i];
		fViewDir = gViewDir[i];
		fLightDir = gLightDir[i];
		fParticleIndex = gParticleIndex[i];
		fParticleSize = gParticleSize[i];
		fColor = vec4(1.0, 0.0, 0.0, 1.0);
		EmitVertex();
	}
	EndPrimitive();	
}
