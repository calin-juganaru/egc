#pragma once

#include <vector>
#include <include/glm.h>
#include <Core/GPU/Mesh.h>

using namespace std;
using namespace glm;

constexpr auto PI = 3.14159265358979323846264338328f;
constexpr auto MAX_WING_POS = 7;

// ================================================================================================

inline Mesh* create_square(std::string name, vec3 corner, float length, vec3 color)
{
	auto vertices = vector<VertexFormat>
	{
		VertexFormat(corner, color),
		VertexFormat(corner + vec3(length, 0, 0), color),
		VertexFormat(corner + vec3(length, length, 0), color),
		VertexFormat(corner + vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	auto indices = vector<unsigned short>{0, 1, 2, 3, 0, 2};
	square->InitFromData(vertices, indices);
	return square;
}

// ================================================================================================

inline Mesh* create_bird(string name, vec3 center, float radius,
						 unsigned short precision, int wing_pos)
{
	auto bird = new Mesh(name);
	auto indices = vector<unsigned short>();
	auto vertices = vector<VertexFormat>();

	auto red = vec3(1, 0, 0);
	auto white = vec3(1, 1, 1);
	auto yellow = vec3(1, 1, 0);
	auto orange = vec3(1, 0.5f, 0);
	auto eye_color = vec3(0, 0, 1);

	auto eye_x_offset = radius - radius / 6.0f;
	auto eye_y_offset = radius / 2 - radius / 15.0f;

	// --------------------------------------------------------------------------------------------

	vertices.push_back(VertexFormat(center + vec3(eye_x_offset, eye_y_offset, 0), eye_color));
	for (auto i = 0; i <= precision; ++i)
	{
		auto theta = 2.0f * PI * float(i) / float(precision);
		auto x = eye_x_offset + radius / 10 * cosf(theta);
		auto y = eye_y_offset + radius / 10 * sinf(theta) - radius / 10.0f;

		vertices.push_back(VertexFormat(center + vec3(x, y, 0), eye_color));
	}

	vertices.push_back(VertexFormat(center + vec3(eye_x_offset, eye_y_offset, 0), white));
	for (auto i = 0; i <= precision; ++i)
	{
		auto theta = 2.0f * PI * float(i) / float(precision);
		auto x = eye_x_offset + radius / 4 * cosf(theta) - radius / 10;
		auto y = eye_y_offset + radius / 4 * sinf(theta) - radius / 10;

		vertices.push_back(VertexFormat(center + vec3(x, y, 0), white));
	}

	vertices.push_back(VertexFormat(center, yellow));
	for (auto i = 0; i <= precision; ++i)
	{
		auto theta = 2.0f * PI * float(i) / float(precision);
		auto x = 1.135f * radius * cosf(theta);
		auto y = radius * sinf(theta);

		vertices.push_back(VertexFormat(center + vec3(x, y, 0), yellow));
	}


	vertices.push_back(VertexFormat(center + vec3(-0.75f * radius, 0, 0), orange));
	vertices.push_back(VertexFormat(center + vec3(0.25f * radius, 0, 0), orange));

	switch (wing_pos % MAX_WING_POS)
	{
		case 0: vertices.push_back(VertexFormat(center + vec3(-0.25f * radius,  0.500f * radius, 0), orange)); break;
		case 1: vertices.push_back(VertexFormat(center + vec3(-0.25f * radius,  0.250f * radius, 0), orange)); break;
		case 2: vertices.push_back(VertexFormat(center + vec3(-0.25f * radius,  0.125f * radius, 0), orange)); break;
		case 3: vertices.push_back(VertexFormat(center + vec3(-0.25f * radius,  0.050f * radius, 0), orange)); break;
		case 4: vertices.push_back(VertexFormat(center + vec3(-0.25f * radius, -0.125f * radius, 0), orange)); break;
		case 5: vertices.push_back(VertexFormat(center + vec3(-0.25f * radius, -0.250f * radius, 0), orange)); break;
		case 6: vertices.push_back(VertexFormat(center + vec3(-0.25f * radius, -0.500f * radius, 0), orange)); break;
		default: break;
	}

	vertices.push_back(VertexFormat(center + vec3(1.10f * radius,  radius / 4, 0), red));
	vertices.push_back(VertexFormat(center + vec3(1.10f * radius, -radius / 4, 0), red));
	vertices.push_back(VertexFormat(center + vec3(1.75f * radius,  radius / 4, 0), red));
	vertices.push_back(VertexFormat(center + vec3(1.10f * radius, -radius / 4, 0), red));
	vertices.push_back(VertexFormat(center + vec3(1.75f * radius,  radius / 4, 0), red));
	vertices.push_back(VertexFormat(center + vec3(1.50f * radius, -radius / 8, 0), red));

	vertices.push_back(VertexFormat(center + vec3(-1.75f * radius,  0.75 * radius, 0), orange));
	vertices.push_back(VertexFormat(center + vec3(-1.75f * radius, -0.75 * radius, 0), orange));
	vertices.push_back(VertexFormat(center + vec3(-radius, 0, 0), orange));


	// --------------------------------------------------------------------------------------------

	for (auto i = 6; i < 15; ++i)
		indices.push_back(3 * precision + i);

	for (auto i = 1; i <= precision; ++i)
	{
		indices.push_back(i);
		indices.push_back(0);
		indices.push_back(i + 1);
	}

	for (auto i = 1; i <= precision; ++i)
	{
		indices.push_back(precision + i + 2);
		indices.push_back(precision + 2);
		indices.push_back(precision + i + 3);
	}

	for (auto i = 1; i <= precision; ++i)
	{
		indices.push_back(2 * precision + i + 4);
		indices.push_back(2 * precision + 4);
		indices.push_back(2 * precision + i + 5);
	}

	for (auto i = 15; i < 18; ++i)
		indices.push_back(3 * precision + i);

	// --------------------------------------------------------------------------------------------

	bird->InitFromData(vertices, indices);
	return bird;
}

// ================================================================================================

inline Mesh* create_sun(string name, vec3 center, float radius, unsigned short precision)
{
	auto sun = new Mesh(name);
	auto indices = vector<unsigned short>();
	auto vertices = vector<VertexFormat>();

	auto yellow = vec3(1, 1, 0);
	auto orange = vec3(1, 0.5f, 0);

	// --------------------------------------------------------------------------------------------

	vertices.push_back(VertexFormat(center, orange));
	for (auto i = 0; i <= precision; ++i)
	{
		auto theta = 2.0f * PI * float(i) / float(precision);
		auto x = radius * cosf(theta);
		auto y = radius * sinf(theta);

		vertices.push_back(VertexFormat(center + vec3(x, y, 0), yellow));
	}

	// --------------------------------------------------------------------------------------------

	for (auto i = 1; i <= precision; ++i)
	{
		indices.push_back(i);
		indices.push_back(0);
		indices.push_back(i + 1);
	}

	// --------------------------------------------------------------------------------------------

	sun->InitFromData(vertices, indices);
	return sun;
}

// ================================================================================================

inline Mesh* create_cloud(string name, vec3 center, float radius, unsigned short precision)
{
	auto cloud = new Mesh(name);
	auto indices = vector<unsigned short>();
	auto vertices = vector<VertexFormat>();
	auto white = vec3(1, 1, 1);

	// --------------------------------------------------------------------------------------------

	vertices.push_back(VertexFormat(center, white));
	for (auto i = 0; i <= precision; ++i)
	{
		auto theta = 2.0f * PI * float(i) / float(precision);
		auto x = radius * cosf(theta);
		auto y = radius * sinf(theta);

		vertices.push_back(VertexFormat(center + vec3(x, y, 0), white));
	}

	center += vec3(radius, 0, 0);
	vertices.push_back(VertexFormat(center, white));
	for (auto i = 0; i <= precision; ++i)
	{
		auto theta = 2.0f * PI * float(i) / float(precision);
		auto x = 0.75f * radius * cosf(theta);
		auto y = 0.75f * radius * sinf(theta) - radius / 5;

		vertices.push_back(VertexFormat(center + vec3(x, y, 0), white));
	}

	center -= vec3(2 * radius, 0, 0);
	vertices.push_back(VertexFormat(center, white));
	for (auto i = 0; i <= precision; ++i)
	{
		auto theta = 2.0f * PI * float(i) / float(precision);
		auto x = 0.75f * radius * cosf(theta);
		auto y = 0.75f * radius * sinf(theta) - radius / 5;

		vertices.push_back(VertexFormat(center + vec3(x, y, 0), white));
	}

	// --------------------------------------------------------------------------------------------

	for (auto i = 1; i <= precision; ++i)
	{
		indices.push_back(i);
		indices.push_back(0);
		indices.push_back(i + 1);
	}

	for (auto i = 1; i <= precision; ++i)
	{
		indices.push_back(precision + i + 2);
		indices.push_back(precision + 2);
		indices.push_back(precision + i + 3);
	}

	for (auto i = 1; i <= precision; ++i)
	{
		indices.push_back(2 * precision + i + 4);
		indices.push_back(2 * precision + 4);
		indices.push_back(2 * precision + i + 5);
	}

	// --------------------------------------------------------------------------------------------

	cloud->InitFromData(vertices, indices);
	return cloud;
}