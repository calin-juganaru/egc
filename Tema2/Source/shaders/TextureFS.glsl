#version 330

uniform sampler2D texture;

in vec2 texture_coord;

layout(location = 0) out vec4 out_color;

void main()
{
	out_color = texture2D(texture, texture_coord);
}