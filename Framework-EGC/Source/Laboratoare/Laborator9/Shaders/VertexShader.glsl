#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float time;
uniform float camera_x;
uniform float camera_y;

out vec2 texture_coord;

void main()
{
	texture_coord = v_texture_coord;

	texture_coord = vec2(texture_coord.x + time / 2, texture_coord.y);
	texture_coord = vec2(texture_coord.x + camera_x / 2, texture_coord.y);
	texture_coord = vec2(texture_coord.x, texture_coord.y + camera_y / 2);

	gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}