
#version 150

//uniform vec4 pickingId;
uniform samplerCube fCube;
uniform float fAlpha;

in vec3 fNormal;
in vec3 fViewDir;
in vec3 fLightDir;
in float fParticleIndex;
in float fParticleSize;
smooth in vec4 fColor;

out vec4 out_Color0;
//out vec4 out_Color1;

void main(void)
{	
	vec3 L = normalize(fLightDir);
	vec3 N = normalize(fNormal);
	vec3 V = normalize(fViewDir);
	vec3 R = normalize(- reflect(L, -N));
	vec3 reflectCoord = normalize(- reflect(V, -N));
	vec3 refractCoord = normalize(- refract(V, -N, 0.85));

	float nDotL = max(0.0, dot(-N, L));
	float rDotV = max(0.0, dot(R, V));
	
	vec4 ambient = vec4(1.0,1.0,1.0,1.0) * vec4(0.4,0.4,0.4,1.0);
	vec4 diffuse = vec4(1.0,1.0,1.0,1.0) * vec4(0.7,0.7,0.7,1.0) * nDotL;
	vec4 specular = vec4(1.0,1.0,1.0,1.0) * vec4(0.5,0.5,0.5,1.0) * pow(rDotV, 40.0) * 0.5;
	vec4 reflectColor = texture(fCube, reflectCoord);
	vec4 refractColor = texture(fCube, refractCoord);
	vec4 texel = vec4(mix(reflectColor.rgb, refractColor.rgb, -dot(V, -N)), 1.0);
	
	float gloss = 0.6;
	
	out_Color0 = fColor + ((ambient + diffuse + (gloss * specular)) + (texel*0.7)) * 0.0000001 + fParticleIndex*0.00001 + fParticleSize*0.00001;
	out_Color0.a = fAlpha;
	//out_Color1 = pickingId;
}
