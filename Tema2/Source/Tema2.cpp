#include "Tema2.h"

deque<pair<float, float>> Clouds, Cans;
deque<pair<pair<float, float>, int>> Obstacles;
unordered_map<string, Texture2D*> mapTextures;

auto FUEL_LOSS_SPEED = 0.0015f;

// ================================================================================================

void Tema2::AddMesh(const string& name, bool obj_or_fbx = false)
{
	Mesh* mesh = new Mesh(name);
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", 
					name + ((obj_or_fbx) ? ".fbx" : ".obj"));
	meshes[mesh->GetMeshID()] = mesh;
}

// ================================================================================================

void Tema2::AddShader(const string& name)
{
	auto shader = new Shader(name.c_str());
	shader->AddShader("Source/shaders/" + name + "VS.glsl", GL_VERTEX_SHADER);
	shader->AddShader("Source/shaders/" + name + "FS.glsl", GL_FRAGMENT_SHADER);
	shader->CreateAndLink();
	shaders[shader->GetName()] = shader;
}

// ================================================================================================

void Tema2::RenderCloud(float x_offset, float y_offset)
{
	for (auto i = 0; i < CLOUD_COMPONENTS; ++i)
	{
		auto modelMatrix = mat4(1); 
		modelMatrix = scale(modelMatrix, vec3(0.25f));
		modelMatrix = translate(modelMatrix, vec3(x_offset + (1 << i) / 1.75f, y_offset, 0.25f));
		modelMatrix = scale(modelMatrix, vec3(1.0f / (CLOUD_COMPONENTS - i)));
		RenderSimpleMesh(meshes["GeoSphere"], shaders["Simple"], modelMatrix, vec3(1));
	}
	for (auto i = CLOUD_COMPONENTS - 1; i > 0; --i)
	{
		auto modelMatrix = mat4(1);
		modelMatrix = scale(modelMatrix, vec3(0.25f));
		modelMatrix = translate(modelMatrix, vec3(x_offset + 2.75f + (1 << (i - 1)) / 1.75f, y_offset, 0.25f));
		modelMatrix = scale(modelMatrix, vec3(1.0f / (i + 1)));
		RenderSimpleMesh(meshes["GeoSphere"], shaders["Simple"], modelMatrix, vec3(1));
	}
}

// ================================================================================================

void Tema2::RenderObstacle(float x_offset, float y_offset)
{
	for (auto i = 1; i < CLOUD_COMPONENTS; ++i)
	{
		auto modelMatrix = rotate(mat4(1), -RADIANS(i * 2.5f), FORWARD);
		modelMatrix = translate(modelMatrix, vec3(x_offset + 0.1f * i, y_offset, 0));
		modelMatrix = scale(modelMatrix, vec3(0.025f * (CLOUD_COMPONENTS - 1) / i));
		RenderSimpleMesh(meshes["GeoSphere"], shaders["Simple"], modelMatrix, vec3(0.9f, 0.2f, 0));

		modelMatrix = rotate(mat4(1), RADIANS(i * 2.5f), FORWARD);
		modelMatrix = translate(modelMatrix, vec3(x_offset - 0.1f * i, y_offset, 0));
		modelMatrix = scale(modelMatrix, vec3(0.025f * (CLOUD_COMPONENTS - 1) / i));
		RenderSimpleMesh(meshes["GeoSphere"], shaders["Simple"], modelMatrix, vec3(0.9f, 0.2f, 0));
	}
	
	auto modelMatrix = translate(mat4(1), vec3(x_offset, y_offset, 0));
	modelMatrix = scale(modelMatrix, vec3(0.05f));
	RenderSimpleMesh(meshes["GeoSphere"], shaders["Simple"], modelMatrix, vec3(0.9f, 0.2f, 0));
}

// ================================================================================================

Tema2::Tema2()
{
	for (auto mesh: {"Plane", "Rotor", "Sea", "GeoSphere", "plane50", "box"})
		AddMesh(mesh, false);

	for (auto mesh: {"Heineken", "Liver", "BluePill" , "GreenPill"})
		AddMesh(mesh, true);

	for (auto shader : {"Sea", "Simple", "Texture", "Normals"})
		AddShader(shader);

	// --------------------------------------------------------------------------------------------

	camera = new Camera(vec3(0, 3, 3), vec3(0, 3, 0), UP);
	planeX = INITIAL_X; planeY = INITIAL_Y;
	materialKd = materialKs = 0.75f;
	materialShininess = 30;
	lightPosition = vec3(1);
	game_over = camera_mode = pause = false;
	lives = MAX_LIVES;
	acceleration = 1; round = 0;

	auto texture = new Texture2D();
	texture->Load2D("Source/Heineken.jpg", GL_REPEAT);
	mapTextures["Heineken"] = texture;

	// --------------------------------------------------------------------------------------------

	projectionMatrix = perspective(RADIANS(45), window->props.aspectRatio, 0.01f, 100.0f);
}

void Tema2::Init()
{
	fuel = 1; 
	difficulty = 1;
	protection = 0;

	Clouds.clear();
	Obstacles.clear();
	Cans.clear();

	// --------------------------------------------------------------------------------------------

	Clouds.push_back(generate_cloud());
	for (auto i = 1; i < MAX_CLOUDS; ++i)
		Clouds.push_back(generate_cloud(Clouds.back()));
	
	Obstacles.push_back({generate_obstacle(), 0});
	for (auto i = 1; i < MAX_OBSTACLES; ++i)
		Obstacles.push_back({generate_obstacle(Obstacles.back().first), 0});

	Cans.push_back(generate_can());
	for (auto i = 1; i < MAX_CANS; ++i)
		Cans.push_back(generate_can(Cans.back()));
}

// ================================================================================================

void Tema2::FrameStart()
{
	glClearColor(1, 0.75f, 0, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

// ================================================================================================

void Tema2::Update(float deltaTimeSeconds)
{
	if (camera_mode)
		camera->Set(vec3(planeX + 0.2f, planeY + 0.1f, 0), vec3(1, 1.25f * planeY, 0), UP); 
	
	if (pause) return;
	if (!lives) game_over = true;
	
	static auto rotation = 0.0f;
	rotation += deltaTimeSeconds / 2.0f;

	if (Obstacles.size() < MAX_OBSTACLES * 1.5)
		Obstacles.push_back({generate_obstacle(Obstacles.back().first), 0});

	fuel -= acceleration * FUEL_LOSS_SPEED;
	protection -= acceleration * PROTECT_LOSS_SPEED;
	FUEL_LOSS_SPEED += FUEL_LOSS_ACC;
	acceleration += 0.00075f; ++round;

	// --------------------------------------------------------------------------------------------

	if (fuel < 0.001f) { --lives; fuel = 1; }

	for (auto& can: Cans)
		if ((planeX < can.first + 0.1f) &&
			(planeX > can.first - 0.1f) &&
			(planeY < can.second + 0.2f) &&
			(planeY > can.second - 0.2f))
		{
			can = {MIN_CAN_X, 0};
			fuel = 1; break;
		}

	// --------------------------------------------------------------------------------------------

	for (auto& obstacle: Obstacles)
		if (obstacle.second
			&& (planeX < obstacle.first.first  + 0.1f)
			&& (planeX > obstacle.first.first  - 0.1f)
			&& (planeY < obstacle.first.second + 0.1f)
			&& (planeY > obstacle.first.second - 0.1f))
		{ 
			if (obstacle.second == 2)
				protection = 1;
			else if (obstacle.second == 1)
				if (lives < MAX_LIVES)
					++lives;

			obstacle.first = {MIN_OBSTACLE_X - 1, 0};
			break; 
		}

	if (protection < 0.01f)
		for (auto& obstacle: Obstacles)
			if (!obstacle.second
			 && (planeX < obstacle.first.first  + 0.675f)
			 && (planeX > obstacle.first.first  - 0.675f)
			 && (planeY < obstacle.first.second + 0.15f)
			 && (planeY > obstacle.first.second - 0.15f))
			{ 
				obstacle.first = {MIN_OBSTACLE_X - 1, 0};
				--lives; break;
			}

	// --------------------------------------------------------------------------------------------
	
	{
		auto modelMatrix = mat4(1.0f);
		modelMatrix = translate(modelMatrix, vec3(planeX, planeY, 0));
		modelMatrix = scale(modelMatrix, vec3(0.1f));
		modelMatrix = rotate(modelMatrix, 0.5f * (planeY - AVG_Y), FORWARD);
		modelMatrix = rotate(modelMatrix, RADIANS(90), RIGHT);
		modelMatrix = rotate(modelMatrix, RADIANS(180), FORWARD);

		if (game_over)
		{
			modelMatrix = rotate(modelMatrix, RADIANS(90), UP);
			modelMatrix = rotate(modelMatrix, 0.5f * (planeY - AVG_Y), UP);
			modelMatrix = rotate(modelMatrix, 2.5f * planeY, RIGHT);
			planeY -= FALL_SPEED;
		}

		if (planeY < MIN_PLANE_Y / 2.5f) { pause = true; this->Init(); }

		RenderSimpleMesh(meshes["Plane"], shaders["Normals"], modelMatrix);
		modelMatrix = rotate(modelMatrix, 17.5f * rotation * acceleration, RIGHT);
		RenderSimpleMesh(meshes["Rotor"], shaders["Normals"], modelMatrix);
	}
	
	// --------------------------------------------------------------------------------------------
	
	{
		auto modelMatrix = mat4(1);
		modelMatrix = translate(modelMatrix, vec3(0, -0.25f, -1.5f));
		modelMatrix = rotate(modelMatrix, RADIANS(90), RIGHT);
		modelMatrix = rotate(modelMatrix, acceleration * rotation, UP);
		RenderSimpleMesh(meshes["Sea"], shaders["Sea"], modelMatrix, vec3(0, 0, 1));
	}

	// --------------------------------------------------------------------------------------------

	{
		if (camera_mode)
		{
			auto modelMatrix = rotate(mat4(1.0f), RADIANS(-90), UP);
			modelMatrix = translate(modelMatrix, vec3(planeX + 1.8f, planeY + 1.05f, -0.9f));
			modelMatrix = rotate(modelMatrix, RADIANS(-90), RIGHT);
			modelMatrix = scale(modelMatrix, vec3(0.0035f, 0.0001f, 0.0035f));
			modelMatrix = rotate(modelMatrix, -0.5f * (planeY - AVG_Y), RIGHT);
			RenderTexturedMesh(meshes["Heineken"], shaders["Texture"],
				modelMatrix, mapTextures["Heineken"]);
		}
		else
		{
			auto modelMatrix = mat4(1.0f);
			modelMatrix = translate(modelMatrix, vec3(1.275f, 3.5f, 1.125f));
			modelMatrix = rotate(modelMatrix, RADIANS(-90), RIGHT);
			modelMatrix = scale(modelMatrix, vec3(0.0035f, 0.0001f, 0.0035f));
			RenderTexturedMesh(meshes["Heineken"], shaders["Texture"],
				modelMatrix, mapTextures["Heineken"]); 
		}
	}

	// --------------------------------------------------------------------------------------------

	if (!game_over && fuel > 0.001f)
	{
		auto modelMatrix = mat4(1.0f);
		modelMatrix = translate(modelMatrix, vec3(1.15f, 3.605f - (1.0f - fuel) / 9.0f, 1.125f));
		modelMatrix = scale(modelMatrix, vec3(0.05f, 0.235f, 0.0001f));
		modelMatrix = scale(modelMatrix, vec3(1, fuel, 1));
		RenderSimpleMesh(meshes["box"], shaders["Simple"], modelMatrix, vec3(0));
	}

	// --------------------------------------------------------------------------------------------

	if (Clouds.front().first < MIN_CLOUD_X)
	{
		Clouds.pop_front();
		Clouds.push_back(generate_cloud(Clouds.back()));
	}

	for (auto& cloud: Clouds)
	{
		cloud.first += acceleration * MoveCloudX();
		cloud.second += MoveCloudY();
		if (cloud.first < MAX_CLOUD_X)
			RenderCloud(cloud.first, cloud.second);
	}
	
	// --------------------------------------------------------------------------------------------

	if (Obstacles.front().first.first < MIN_OBSTACLE_X)
	{
		Obstacles.pop_front();
		auto aux = static_cast<int>(round * 2) % 3;
		if (lives == MAX_LIVES && aux == 1) aux = 0;
		Obstacles.push_back({generate_obstacle(Obstacles.back().first), aux});
	}

	for (auto& obstacle: Obstacles)
	{
		obstacle.first.first += acceleration * MoveObstacleX();
		obstacle.first.second += MoveObstacleY();

		if (obstacle.first.first < MAX_OBSTACLE_X && obstacle.first.first > MIN_OBSTACLE_X)
		{
			if (obstacle.second)
			{
				auto modelMatrix = mat4(1.0f);
				modelMatrix = translate(modelMatrix, vec3(obstacle.first.first, obstacle.first.second, 0));
				modelMatrix = rotate(modelMatrix, RADIANS(90), UP);
				modelMatrix = rotate(modelMatrix, obstacle.first.first, RIGHT);
				modelMatrix = scale(modelMatrix, vec3(0.35f, 0.35f, 0.35f));

				if (obstacle.second == 2)
					 RenderSimpleMesh(meshes["BluePill"], shaders["Simple"], modelMatrix, BLUE_PILL_COLOR);
				else RenderSimpleMesh(meshes["GreenPill"], shaders["Simple"], modelMatrix, 0.5f * UP);
			}
			else RenderObstacle(obstacle.first.first, obstacle.first.second);
		}
	}

	// --------------------------------------------------------------------------------------------

	if (Cans.front().first < MIN_CAN_X)
	{
		Cans.pop_front();
		Cans.push_back(generate_obstacle(Cans.back()));
	}

	for (auto& can: Cans)
	{
		can.first += acceleration * MoveCanX();
		can.second += MoveCanY();
		
		if (can.first < MAX_CAN_X && can.first > MIN_CAN_X)
		{
			auto modelMatrix = mat4(1.0f);
			modelMatrix = translate(modelMatrix, vec3(can.first, can.second, -0.1f));
			modelMatrix = rotate(modelMatrix, RADIANS(-90), RIGHT);
			modelMatrix = scale(modelMatrix, vec3(0.003f));
			RenderTexturedMesh(meshes["Heineken"], shaders["Texture"],
								modelMatrix, mapTextures["Heineken"]);
		}
	}

	// --------------------------------------------------------------------------------------------
	
	for (auto i = 0; i < lives; ++i)
	{
		if (camera_mode)
		{
			auto modelMatrix = rotate(mat4(1.0f), RADIANS(-90), UP);
			auto aux = 0.3f * static_cast<float>(i - 1);
			modelMatrix = translate(modelMatrix, vec3(planeX + aux, planeY + 1.25f, -0.9f));
			modelMatrix = scale(modelMatrix, vec3(0.3f, 0.3f, 0.001f));
			modelMatrix = rotate(modelMatrix, 0.5f * (planeY - AVG_Y), RIGHT);
			RenderSimpleMesh(meshes["Liver"], shaders["Simple"], modelMatrix, vec3(1, 0, 0.35f));
		}
		else
		{
			auto modelMatrix = mat4(1.0f);
			auto aux = 0.3f * static_cast<float>(i);
			modelMatrix = translate(modelMatrix, vec3(-1.8f + aux, 3.975f, 0.31f));
			modelMatrix = scale(modelMatrix, vec3(0.3f, 0.3f, 0.001f));
			RenderSimpleMesh(meshes["Liver"], shaders["Simple"], modelMatrix, vec3(1, 0, 0.35f));
		}
	}

	// --------------------------------------------------------------------------------------------

	if (protection > 0.01f && !game_over)
	{
		{
			auto modelMatrix = mat4(1.0f);
			modelMatrix = translate(modelMatrix, vec3(1.025f, 3.7225f, 1.125f));
			modelMatrix = rotate(modelMatrix, RADIANS(90), UP);
			modelMatrix = rotate(modelMatrix, RADIANS(90), RIGHT);
			modelMatrix = scale(modelMatrix, vec3(0.001f, 0.45f, 0.45f));
			RenderSimpleMesh(meshes["BluePill"], shaders["Simple"], modelMatrix, BLUE_PILL_COLOR);
		}

		{
			auto modelMatrix = mat4(1.0f);
			modelMatrix = translate(modelMatrix, vec3(0.9f, 3.605f - (1.0f - protection) / 9.0f, 1.125f));
			modelMatrix = scale(modelMatrix, vec3(0.05f, 0.235f, 0.0001f));
			modelMatrix = scale(modelMatrix, vec3(1, protection, 1));
			RenderSimpleMesh(meshes["box"], shaders["Simple"], modelMatrix, vec3(0));
		}
	}
}

// ================================================================================================

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

// ================================================================================================

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const mat4& modelMatrix, const vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID()) return;

	glUseProgram(shader->program);

	auto aux = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(aux, 1, value_ptr(lightPosition));

	aux = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(aux, materialShininess);

	aux = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(aux, materialKd);

	aux = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(aux, materialKs);

	aux = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(aux, 1, value_ptr(color));

	aux = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(aux, 1, GL_FALSE, value_ptr(modelMatrix));

	aux = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(aux, 1, GL_FALSE, value_ptr(camera->GetViewMatrix()));

	aux = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(aux, 1, GL_FALSE, value_ptr(projectionMatrix));

	glBindVertexArray(mesh->GetBuffers()->VAO); mesh->Render();
}

// ================================================================================================

void Tema2::RenderTexturedMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, Texture2D* texture)
{
	if (!mesh || !shader || !shader->GetProgramID()) return;

	glUseProgram(shader->program);

	auto aux = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(aux, 1, GL_FALSE, value_ptr(modelMatrix));

	aux = glGetUniformLocation(shader->program, "View");
	glUniformMatrix4fv(aux, 1, GL_FALSE, value_ptr(camera->GetViewMatrix()));

	aux = glGetUniformLocation(shader->program, "Projection");
	glUniformMatrix4fv(aux, 1, GL_FALSE, value_ptr(projectionMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
	glUniform1i(glGetUniformLocation(shader->GetProgramID(), "texture"), 0);

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), mesh->indices.size(), GL_UNSIGNED_SHORT, 0);
}

// ================================================================================================

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (window->KeyHold(GLFW_KEY_W)) camera->TranslateForward(0.1f);
	if (window->KeyHold(GLFW_KEY_A)) camera->TranslateRight(-0.1f);
	if (window->KeyHold(GLFW_KEY_S)) camera->TranslateForward(-0.1f);
	if (window->KeyHold(GLFW_KEY_D)) camera->TranslateRight(0.1f);
	if (window->KeyHold(GLFW_KEY_Q)) camera->TranslateUpword(-0.1f);
	if (window->KeyHold(GLFW_KEY_E)) camera->TranslateUpword(0.1f);
}

// ================================================================================================

void Tema2::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_C)
	{
		camera_mode = !camera_mode;
		if (camera_mode)
			 camera->Set(vec3(planeX + 0.2f, planeY + 0.1f, 0), vec3(1, 1.25f * planeY, 0), UP);
		else camera->Set(vec3(0, 3, 3), vec3(0, 3, 0), UP);
	}

	if (key == GLFW_KEY_R)
	{
		this->Init();
		lives = MAX_LIVES;
		planeY = INITIAL_Y;
		game_over = false;
		pause = false;
		acceleration = 1;
		round = 0;
	}

	if (key == GLFW_KEY_P)
		pause = !pause;
}

// ================================================================================================

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	if (!game_over)
	{
		planeY -= deltaY / 300.0f;
		planeY = min(planeY, MAX_PLANE_Y);
		planeY = max(planeY, MIN_PLANE_Y);
	}
}

// ================================================================================================

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	acceleration *= 2.0f;
}

// ================================================================================================

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	acceleration *= 0.5f;
}

// ================================================================================================