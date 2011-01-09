
#version 140

uniform mat4 projMat;
uniform mat4 mvMat;
uniform mat4 normMat;

in vec3 vVertex;
in vec3 vNormal;

out vec3 fNormal;
out vec3 fViewDir;
out vec3 fLightDir;

vec3 calcTangent(in vec3 normal)
{
	vec3 tangent;
	vec3 c1 = cross(normal, vec3(0.0, 0.0, 1.0)); 
	vec3 c2 = cross(normal, vec3(0.0, 1.0, 0.0)); 
	if(length(c1) > length(c2))
	{
		tangent = c1;	
	}
	else
	{
		tangent = c2;	
	}
	return tangent;
}

void main(void)
{	
	gl_Position = projMat * mvMat * vec4(vVertex, 1.0);
	
	fNormal = normalize(normMat * vec4(vNormal, 1.0)).xyz;
	
	vec3 n = normalize(normMat * vec4(vNormal, 1.0)).xyz;
	vec3 t = normalize(normMat * vec4(calcTangent(vNormal), 1.0)).xyz;
	vec3 b = cross(n, t);
	
	mat3 tbnMat = mat3(t.x, b.x, n.x,
                          t.y, b.y, n.y,
                          t.z, b.z, n.z);
    
    //Transform view dir to eye space (inverse of any vertex that has been transformed to eye space), then to tangent space
	vec4 pos = mvMat * vec4(vVertex, 1.0);
	fViewDir = -(pos.xyz / pos.w);
	fViewDir = tbnMat * fViewDir;
	
	//Transform light dir to eye space, then to tangent space
	//vec4 lightDir = vec4(PointLight1.position, 1.0) - pos;
	fLightDir = vec3(mvMat * vec4(0.0, 0.0, 0.0, 1.0));
	fLightDir = tbnMat * fLightDir;
}
