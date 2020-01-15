#pragma once
#include <include/glm.h>
#include <include/math.h>

namespace Laborator
{
	class Camera
	{
		public:
			Camera()
			{
				position = glm::vec3(0, 2, 5);
				forward = glm::vec3(0, 0, -1);
				up		= glm::vec3(0, 1, 0);
				right	= glm::vec3(1, 0, 0);
				distanceToTarget = 2;
			}

			Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				Set(position, center, up);
			}

			~Camera() = default;

			void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
			{
				this->position = position;
				forward = glm::normalize(center - position);
				right	= glm::cross(forward, up);
				this->up = glm::cross(right,forward);
			}

			void MoveForward(float distance)
			{
				auto direction = glm::normalize(glm::vec3(forward.x, 0, forward.z));
				position += distance * direction;
			}

			void TranslateForward(float distance)
			{
				position += distance * forward;
			}

			void TranslateUpword(float distance)
			{
				position += distance * up;
			}

			void TranslateRight(float distance)
			{
				position += distance * right;
			}

			void RotateFirstPerson_OX(float angle)
			{
				auto aux = glm::rotate(glm::mat4(1), angle, glm::vec3(1, 0, 0)) * glm::vec4(forward, 0);
				forward = glm::normalize(glm::vec3(aux));
				up = glm::cross(right, forward);
			}

			void RotateFirstPerson_OY(float angle)
			{
				auto aux = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 0);
				forward = glm::normalize(glm::vec3(aux));

				aux = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 0);
				right = glm::normalize(glm::vec3(aux));

				up = glm::cross(right, forward);
			}

			void RotateFirstPerson_OZ(float angle)
			{
				auto aux = glm::rotate(glm::mat4(1), angle, glm::vec3(0, 0, 1)) * glm::vec4(right, 0);
				right = glm::normalize(glm::vec3(aux));

				up = glm::cross(right, forward);
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

			glm::mat4 GetViewMatrix()
			{
				return glm::lookAt(position, position + forward, up);
			}

			glm::vec3 GetTargetPosition()
			{
				return position + forward * distanceToTarget;
			}

		public:
			float distanceToTarget;
			glm::vec3 position;
			glm::vec3 forward;
			glm::vec3 right;
			glm::vec3 up;
		};
}