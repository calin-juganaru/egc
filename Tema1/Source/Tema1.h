#pragma once

#include <iostream>
#include <deque>
#include <tuple>
#include <random>

#include <Component/SimpleScene.h>
#include <Core/Engine.h>
#include <include/glm.h>

using namespace std;
using namespace glm;

using wall_mesh_t = tuple<Mesh*, float, float, bool>;

constexpr auto MAX_WALLS  = 11;
constexpr auto MAX_COLORS = 100;
constexpr auto PRECISION  = 64;

constexpr auto INITIAL_X = 100.0f;
constexpr auto INITIAL_Y = 400.0f;

constexpr auto JUMP_HEIGHT = 50.0f;
constexpr auto WALL_DISTANCE = 125.0f;
constexpr auto WALL_WIDTH  = 75.0f;
constexpr auto WALL_SCALE  = 7.5f;

constexpr auto BIRD_WIDTH  = WALL_WIDTH;
constexpr auto BIRD_HEIGHT = WALL_WIDTH / 2;

constexpr auto MAX_GAP = 275.0f;
constexpr auto MIN_GAP = 225.0f;

constexpr auto SLIDE_HEIGHT = 50.0f;
constexpr auto SLIDE_RATIO  = SLIDE_HEIGHT / WALL_WIDTH;
constexpr auto SLIDING_FREQ = 5;

constexpr auto FLY_SPEED_INC  = 0.02f;
constexpr auto FALL_SPEED_INC = 0.01f;
constexpr auto FALL_ANGLE_INC = 0.1f;

class Tema1: public SimpleScene
{
public:
	Tema1() = default;
	~Tema1() = default;

	void Init() override;

private:
	void FrameStart() override;
	void Update(float time) override;
	void OnKeyPress(int key, int mods) override;
	Mesh* generate_wall(int index);
	float random_scale(bool sliding, int max);

	mat3 model;
	float X = INITIAL_X, Y;
	int score;
	deque<wall_mesh_t> walls;
	bool end_game, pause;
	float wing_pos, angle;

	float FALL_SPEED, FLY_SPEED;
};

inline mat3 Translate(float X, float Y = 0)
{
	return mat3(1, 0, 0, 0, 1, 0, X, Y, 1);
}

inline mat3 Scale(float X, float Y = 1)
{
	return mat3(X, 0, 0, 0, Y, 0, 0, 0, 1);
}

inline mat3 Rotate(float angle)
{
	auto _cos = cos(angle), _sin = sin(angle);
	return mat3(_cos, _sin, 0, -_sin, _cos, 0, 0, 0, 1);
}