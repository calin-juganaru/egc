#pragma once

#include <Component/Transform/Transform.h>
#include <Component/SimpleScene.h>

#include <Camera.h>
#include "Meshes.h"

#include <iostream>
#include <vector>
#include <deque>

// ================================================================================================

constexpr auto PI = 3.14159265357f;

constexpr auto MIN_PLANE_Y = 2.65f;
constexpr auto MAX_PLANE_Y = 4.125f;
constexpr auto AVG_Y = 0.5f * (MIN_PLANE_Y + MAX_PLANE_Y);

constexpr auto INITIAL_X = -0.75f;
constexpr auto INITIAL_Y = 3.0f;

constexpr auto MAX_LIVES = 3;
constexpr auto FALL_SPEED = 0.025f;

constexpr auto FUEL_LOSS_ACC = 0.0000005f;
constexpr auto PROTECT_LOSS_SPEED = 0.0025;

constexpr auto BLUE_PILL_COLOR = vec3(0.5f, 0.9f, 1);

// ================================================================================================

class Tema2: public SimpleScene
{
public:
	Tema2(); ~Tema2() = default;
	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void AddMesh(const string& name, bool obj_or_fbx);
	void AddShader(const string& name);
	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const mat4& modelMatrix, const vec3& color = vec3(1));
	void RenderTexturedMesh(Mesh* mesh, Shader* shader, const mat4& modelMatrix, Texture2D* texture);
	void RenderCloud(float x_offset, float y_offset);
	void RenderObstacle(float x_offset, float y_offset);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;

protected:
	Camera* camera;
	mat4 projectionMatrix;
	bool game_over, camera_mode, pause;
	vec3 lightPosition;

	unsigned materialShininess, lives, round;
	float materialKd, materialKs;
	float acceleration, difficulty, protection;
	float planeX, planeY, planeZ, fuel;
};

// ================================================================================================