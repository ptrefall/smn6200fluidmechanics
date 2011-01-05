
#version 140

//uniform vec4 pickingId;

in vec4 fColor;

out vec4 out_Color0;
//out vec4 out_Color1;

void main(void)
{	
	out_Color0 = fColor;
	
	//out_Color1 = pickingId;
}
