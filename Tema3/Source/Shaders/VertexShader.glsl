#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform sampler2D texture;

uniform float time;
uniform float camera_x;
uniform float camera_y;

out vec2 texture_coord;
out vec3 normal;

void main()
{
	texture_coord = v_texture_coord;

	//texture_coord = vec2(texture_coord.x + time / 2, texture_coord.y);
	//texture_coord = vec2(texture_coord.x + camera_x / 2, texture_coord.y);
	//texture_coord = vec2(texture_coord.x, texture_coord.y + camera_y / 2);
	
	float u = texture_coord.x, v = texture_coord.y;
	float height = texture2D(texture, vec2(u, v)).r;
	ivec2 size = textureSize(texture, 0);
	vec2 texelSize = vec2(1.0 / size.x, 1.0 / size.y);

	float heightRight = texture2D(texture, vec2(u + texelSize.x, v)).r;
	float heightUp = texture2D(texture, vec2(u, v + texelSize.y)).r;

	float Hx = height - heightRight;
	float Hz = height - heightUp;

	normal = normalize(vec3(Hx, 1, Hz));
	vec3 new_pos = v_position + vec3(0, 3.75f * height, 0);
	gl_Position = Projection * View * Model * vec4(new_pos, 1.0);
}