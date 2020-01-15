#pragma once

#include <Core/GPU/Mesh.h>
#include <Core/Engine.h>

#include <random>
#include <algorithm>

constexpr auto CLOUD_COMPONENTS = 3;
constexpr auto MAX_CLOUDS = 3;
constexpr auto MAX_OBSTACLES = 4;
constexpr auto MAX_CANS = 4;

constexpr auto MIN_CLOUD_X = -15.0f;
constexpr auto MAX_CLOUD_X = 12.25f;
constexpr auto MIN_CLOUD_Y = 12.25f;
constexpr auto MAX_CLOUD_Y = 16.5f;
constexpr auto MIN_CLOUD_DIST = 5.25f;

constexpr auto MIN_OBSTACLE_X = -3.125f;
constexpr auto MAX_OBSTACLE_X = 2.5f;
constexpr auto MIN_OBSTACLE_Y = 2.6f;
constexpr auto MAX_OBSTACLE_Y = 4.125f;
constexpr auto MIN_OBSTACLE_DIST = 0.75f;

constexpr auto MIN_CAN_X = -2.3f;
constexpr auto MAX_CAN_X = 2.25f;
constexpr auto MIN_CAN_Y = 2.25f;
constexpr auto MAX_CAN_Y = 3.85f;
constexpr auto MIN_CAN_DIST = 1.05f;

// ================================================================================================

auto square = [](const float& x)
{
	return x * x;
};

auto dist = [](const pair<float, float>& x, const pair<float, float>& y)
{
	return sqrtf(square(x.first - y.first) + square(x.second - y.second));
};

// ================================================================================================

auto MoveCloudX = []()
{
	static random_device rd;
	static auto rand_gen = mt19937(rd());
	static auto rand = uniform_real_distribution<float>(-0.1f, 0);
	return rand(rand_gen);
};

auto MoveCloudY = []()
{
	static random_device rd;
	static auto rand_gen = mt19937(rd());
	static auto rand = uniform_real_distribution<float>(-0.1f, 0.1f);
	return rand(rand_gen);
};

// ================================================================================================

auto generate_cloud = [](pair<float, float> last = { -10.0f, 13.0f })
{
	static random_device rd;
	static auto rand_gen_x = mt19937(rd());
	static auto rand_x = uniform_real_distribution<float>(0, 15);
	static auto rand_gen_y = mt19937(rd());
	static auto rand_y = uniform_real_distribution<float>(-5, 5);

	pair<float, float> aux;
	do
	{
		aux = make_pair(last.first + rand_x(rand_gen_x),
			last.second + rand_y(rand_gen_y));
	}
	while (dist(aux, last) < MIN_CLOUD_DIST ||
			aux.second > MAX_CLOUD_Y ||
			aux.second < MIN_CLOUD_Y);

	return aux;
};

// ================================================================================================

auto MoveObstacleX = []()
{
	static random_device rd;
	static auto rand_gen = mt19937(rd());
	static auto rand = uniform_real_distribution<float>(-0.025f, 0);
	return rand(rand_gen);
};

auto MoveObstacleY = []()
{
	static random_device rd;
	static auto rand_gen = mt19937(rd());
	static auto rand = uniform_real_distribution<float>(-0.01f, 0.01f);
	return rand(rand_gen);
};

// ================================================================================================

auto generate_obstacle = [](pair<float, float> last = {0.0f, 6.0f})
{
	static random_device rd;
	static auto rand_gen_x = mt19937(rd());
	static auto rand_x = uniform_real_distribution<float>(0, 3.25f);
	static auto rand_gen_y = mt19937(rd());
	static auto rand_y = uniform_real_distribution<float>(-2.5f, 1.5f);

	pair<float, float> aux;
	do
	{
		aux = make_pair(last.first + rand_x(rand_gen_x),
			last.second + rand_y(rand_gen_y));
	}
	while (dist(aux, last) < MIN_OBSTACLE_DIST ||
			aux.second > MAX_OBSTACLE_Y ||
			aux.second < MIN_OBSTACLE_Y);

	return aux;
};

// ================================================================================================

auto MoveCanX = []()
{
	static random_device rd;
	static auto rand_gen = mt19937(rd());
	static auto rand = uniform_real_distribution<float>(-0.025f, 0);
	return rand(rand_gen);
};

auto MoveCanY = []()
{
	static random_device rd;
	static auto rand_gen = mt19937(rd());
	static auto rand = uniform_real_distribution<float>(-0.01f, 0.01f);
	return rand(rand_gen);
};

// ================================================================================================

auto generate_can = [](pair<float, float> last = {0.0f, 6.0f})
{
	static random_device rd;
	static auto rand_gen_x = mt19937(rd());
	static auto rand_x = uniform_real_distribution<float>(0, 3.25f);
	static auto rand_gen_y = mt19937(rd());
	static auto rand_y = uniform_real_distribution<float>(-2.5f, 1.5f);

	pair<float, float> aux;
	do
	{
		aux = make_pair(last.first + rand_x(rand_gen_x),
			last.second + rand_y(rand_gen_y));
	}
	while (dist(aux, last) < MIN_CAN_DIST ||
			aux.second > MAX_CAN_Y ||
			aux.second < MIN_CAN_Y);

	return aux;
};

// ================================================================================================