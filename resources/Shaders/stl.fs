
#version 140

//uniform vec4 pickingId;

in vec3 fNormal;
in vec3 fViewDir;
in vec3 fLightDir;

out vec4 out_Color0;
//out vec4 out_Color1;

void main(void)
{	
	vec3 L = normalize(fLightDir);
	vec3 N = normalize(fNormal);
	vec3 V = normalize(fViewDir);
	vec3 R = normalize(- reflect(L, -N));

	float nDotL = max(0.0, dot(-N, L));
	float rDotV = max(0.0, dot(R, V));
	
	vec4 ambient = vec4(1.0,1.0,1.0,1.0) * vec4(0.4,0.4,0.4,1.0);
	vec4 diffuse = vec4(1.0,1.0,1.0,1.0) * vec4(0.7,0.7,0.7,1.0) * nDotL;
	vec4 specular = vec4(1.0,1.0,1.0,1.0) * vec4(0.5,0.5,0.5,1.0) * pow(rDotV, 40.0) * 0.5;
	
	float gloss = 0.6;
	
	out_Color0 = (ambient + diffuse + (gloss * specular)) * vec4(1.0, 0.2, 0.2, 1.0);
	//out_Color1 = pickingId;
}
