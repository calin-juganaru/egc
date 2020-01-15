#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

out vec3 color;

float random(vec3 v) 
{
    return fract(sin(dot(v, vec3(12.98765, 78.23451, 1.23456))) * 43758.54321);
}

void main()
{
	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
	vec3 world_normal = normalize(mat3(Model) * v_normal);

	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);

	float ambient_light = 0.35;
	float diffuse_light = material_kd * max(dot(N, L), 0.0);
	float specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);

	float light = ambient_light + (specular_light + diffuse_light) / pow(distance(light_position, world_pos), 2);

	color = light * vec3(0, 0, 0.75) + vec3(0, 0, random(v_normal));
	vec3 new_pos = v_position + 0.35 * vec3(random(v_position));
	gl_Position = Projection * View * Model * vec4(new_pos, 1.0);
}
