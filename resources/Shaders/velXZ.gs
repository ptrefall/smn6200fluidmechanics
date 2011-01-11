
#version 150
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
layout(line_strip, max_vertices = 4) out;

uniform mat4 projMat;
uniform mat4 mvMat;
uniform float vShowInHalf;

in float gScale[];
in vec4 gVelocity[];
in vec3 gNormal[];
in vec3 gViewDir[];
in vec3 gLightDir[];

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
	float discard_this;
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
		fColor = vec4(1.0, 0.0, 0.0, 1.0);
		EmitVertex();
		
		gl_Position = projMat * mvMat * (gl_in[i].gl_Position + (gVelocity[i]*gScale[i]*0.05));
		fNormal = gNormal[i];
		fViewDir = gViewDir[i];
		fLightDir = gLightDir[i];
		fColor = vec4(0.0, 0.0, 1.0, 1.0);
		EmitVertex();
	}
	EndPrimitive();
	
	//From point to line, mirrored on X axis
	for(i=0; i< gl_in.length(); i++){
		gl_Position = projMat * mvMat * vec4(gl_in[i].gl_Position.x*-1.0, gl_in[i].gl_Position.yzw);
		fNormal = gNormal[i];
		fViewDir = gViewDir[i];
		fLightDir = gLightDir[i];
		fColor = vec4(1.0, 0.0, 0.0, 1.0);
		EmitVertex();
		
		gl_Position = projMat * mvMat * (vec4(gl_in[i].gl_Position.x*-1.0, gl_in[i].gl_Position.yzw) + (vec4(gVelocity[i].x*-1.0, gVelocity[i].yzw)*gScale[i]*0.05));
		fNormal = gNormal[i];
		fViewDir = gViewDir[i];
		fLightDir = gLightDir[i];
		fColor = vec4(0.0, 0.0, 1.0, 1.0);
		EmitVertex();
	}
	EndPrimitive();	
}
