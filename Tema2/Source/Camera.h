#pragma once

#include <include/glm.h>
#include <include/math.h>

using namespace glm;
using namespace std;

constexpr auto RIGHT   = vec3(1, 0, 0);
constexpr auto UP      = vec3(0, 1, 0);
constexpr auto FORWARD = vec3(0, 0, 1);

class Camera
{
public:
	float distanceToTarget;
	vec3 position, right, up, forward;

	Camera():
		right(RIGHT), up(UP), forward(FORWARD), 
		distanceToTarget(2), position(vec3(0, 2, 5)) {}

	Camera(const vec3& position, const vec3& center, const vec3& up)
	{
		Set(position, center, up);
	}

	~Camera() = default;

	void Set(const vec3& position, const vec3& center, const vec3& up)
	{
		this->position = position;
		forward = normalize(center - position);
		right = cross(forward, up);
		this->up = cross(right, forward);
	}

	void MoveForward(float distance)
	{
		auto direction = normalize(vec3(forward.x, 0, forward.z));
		position += distance * direction;
	}

	void TranslateForward(float distance)
	{
		position += distance * normalize(forward);
	}

	void TranslateUpword(float distance)
	{
		position += distance * normalize(up);
	}

	void TranslateRight(float distance)
	{
		position += distance * normalize(right);
	}

	void RotateFirstPerson_OX(float angle)
	{
		auto aux = rotate(mat4(1), angle, vec3(1, 0, 0)) * vec4(forward, 0);
		forward = normalize(vec3(aux));
		up = normalize(cross(right, forward));
	}

	void RotateFirstPerson_OY(float angle)
	{
		auto aux = rotate(mat4(1), angle, UP) * vec4(forward, 0);
		forward = normalize(vec3(aux));

		aux = rotate(mat4(1), angle, UP) * vec4(right, 0);
		right = normalize(vec3(aux));

		up = normalize(cross(right, forward));
	}

	void RotateFirstPerson_OZ(float angle)
	{
		auto aux = rotate(mat4(1), angle, FORWARD) * vec4(right, 0);
		right = normalize(vec3(aux));

		up = normalize(cross(right, forward));
	}

	void RotateThirdPerson_OX(float angle)
	{
		TranslateRight(-distanceToTarget);
		RotateFirstPerson_OX(angle);
		TranslateRight(distanceToTarget);
	}

	void RotateThirdPerson_OY(float angle)
	{
		TranslateUpword(-distanceToTarget);
		RotateFirstPerson_OY(angle);
		TranslateUpword(distanceToTarget);
	}

	void RotateThirdPerson_OZ(float angle)
	{
		TranslateForward(-distanceToTarget);
		RotateFirstPerson_OZ(angle);
		TranslateForward(distanceToTarget);
	}

	mat4 GetViewMatrix()
	{
		return lookAt(position, position + forward, up);
	}

	vec3 GetTargetPosition()
	{
		return position + forward * distanceToTarget;
	}
};