#version 330
 
uniform sampler2D texture_0;
uniform sampler2D texture_1;
 
in vec2 texture_coord;

layout(location = 0) out vec4 out_color;

void main()
{
	vec4 color0 = texture2D(texture_0, texture_coord);
	vec4 color1 = texture2D(texture_1, texture_coord);
	vec4 color = mix(color0, color1, 0.5f);

	if (color.a < 0.5f) discard;

	out_color = color;
}