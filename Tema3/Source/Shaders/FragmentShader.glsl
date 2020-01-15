#version 330
 
uniform sampler2D texture_0;
uniform sampler2D texture_1;
 
in vec2 texture_coord;
in vec3 normal;

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 color = texture2D(texture_0, texture_coord);

	if (color.a < 0.5f) discard;

	out_color = color;
}