#pragma once

#include <include/glm.h>

namespace Transform2D
{
	inline glm::mat3 Translate(float translateX, float translateY)
	{
		return glm::transpose(glm::mat3
		(
			1, 0, translateX,
			0, 1, translateY,
			0, 0, 1)
		);
	}

	inline glm::mat3 Scale(float scaleX, float scaleY)
	{
		return glm::transpose(glm::mat3
		(
			scaleX, 0, 0,
			0, scaleY, 0,
			0, 0, 1
		));
	}

	inline glm::mat3 Rotate(float radians)
	{
		auto cosu = cos(radians);
		auto sinu = sin(radians);
		return glm::transpose(glm::mat3
		(
			cosu, -sinu, 0,
			sinu, cosu, 0,
			0, 0, 1
		));
	}
}
