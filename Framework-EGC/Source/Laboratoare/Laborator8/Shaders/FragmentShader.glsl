#version 330

in vec3 world_position;
in vec3 world_normal;

uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

uniform int is_spot_light;
uniform float angle;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = reflect(L, normalize(world_normal));
	
	float ambient_light = 0.25 * material_kd;
	float diffuse_light = material_kd * max(dot(L, N), 0);

	float specular_light = 0;
	if (diffuse_light > 0)
		specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);

	float cut_off = radians(angle);
	float spot_light = dot(-L, light_direction);
	float spot_light_limit = cos(cut_off);

	float linear_att = (spot_light - spot_light_limit) / (1 - spot_light_limit);
	float light_att_factor = pow(linear_att, 2);
	float aten_fac = 1;

	if (length(light_position - world_position) > 1)
		aten_fac = 1 / pow(length(light_position - world_position), 2);

	vec3 light;
	if (is_spot_light == 0) 
	{
		light = object_color * (ambient_light + aten_fac * (diffuse_light + specular_light));
	}
	else 
	{
		if (spot_light > spot_light_limit)
			 light = object_color * (ambient_light + (light_att_factor) * (diffuse_light + specular_light));
		else light = ambient_light * object_color;
	}

	out_color = vec4(light, 1);
}