
#version 140

//uniform vec4 pickingId;
uniform sampler2D fTexture;

in vec4 fColor;
smooth in vec2 fTexCoord;

out vec4 out_Color0;
//out vec4 out_Color1;

void main(void)
{	
	vec4 tex_color = texture(fTexture, fTexCoord.st);
	out_Color0 = tex_color+vec4(fColor.rgb*0.0000001, 0.0);
	
	//out_Color1 = pickingId;
}