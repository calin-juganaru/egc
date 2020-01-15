#include "Tema1.h"
#include "Meshes.h"

// ================================================================================================

float Tema1::random_scale(bool sliding = false, int max = INITIAL_Y)
{
	static auto last = 2.5f;
	static auto mod = max;
	static random_device rd;
	static auto rand_gen = mt19937(rd());
	static auto rand = uniform_real_distribution<float>(0, mod);

	float h1, h2, H1, H2, aux;
	do
	{
		aux = rand(rand_gen) / 100.0f;
		h1 = WALL_WIDTH * last, H1 = h1 + MAX_GAP;
		h2 = WALL_WIDTH * aux, H2 = h2 + MAX_GAP;
	}
	while (H1 - h2 < MIN_GAP || H2 - h1 < MIN_GAP);
	last = aux;
	return last;
}

// ================================================================================================

void Tema1::Init()
{
	cout << "\n*******************************\n\n";
	cout << ">>> Your score is 0 ^_^\n";

	end_game = pause = false;
	Y = INITIAL_Y;
	FALL_SPEED = 1.25f;
	FLY_SPEED = 2.5f;
	wing_pos = score = angle = 0;

	auto resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, resolution.x, 0, resolution.y, 0.01f, 100);
	camera->SetPosition(vec3(0, 0, 1));
	camera->SetRotation(vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	// --------------------------------------------------------------------------------------------

	random_device rd;
	auto rand_gen = mt19937(rd());
	auto rand = uniform_real_distribution<float>(0, 1);

	for (auto i = 0; i < MAX_COLORS; ++i)
	{
		auto r = rand(rand_gen), g = rand(rand_gen), b = rand(rand_gen);
		if (r == g && g == b) b = rand(rand_gen);
		auto color = vec3(r, g, b);
		AddMeshToList(create_square("wall" + to_string(i), vec3(), WALL_WIDTH, color));
	}

	for (auto i = 0; i < MAX_WING_POS; ++i)
		AddMeshToList(create_bird("bird" + to_string(i), vec3(), BIRD_HEIGHT, PRECISION, i));

	AddMeshToList(create_sun("sun", vec3(0.9f * resolution.x,
					0.8f * resolution.y, 0), 2.25f * WALL_WIDTH, PRECISION));
	AddMeshToList(create_cloud("cloud", vec3(0.25f * resolution.x,
		0.8f * resolution.y, 0), 1.5f * WALL_WIDTH, PRECISION));

	// --------------------------------------------------------------------------------------------

	walls.push_back({generate_wall(0), WALL_DISTANCE * 4,
					 random_scale(false, 0.8f * resolution.y), false});
	for (auto i = 1; i < MAX_WALLS; ++i)
	{
		walls.push_back
		({
			generate_wall(i),
			WALL_DISTANCE * (i + 4),
			random_scale(), false
		});
	}
}

// ================================================================================================

void Tema1::FrameStart()
{
	glClearColor(0, 1, 1, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	auto resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

// ================================================================================================

Mesh* Tema1::generate_wall(int index)
{
	return meshes["wall" + to_string(index % MAX_COLORS)];
}

// ================================================================================================

void Tema1::Update(float time)
{
	if (pause || end_game) return;
	wing_pos += 0.125f;
	angle -= time / 7.5f;
	Y -= FALL_SPEED;

	// --------------------------------------------------------------------------------------------

	for (auto& wall: walls)
	{
		if (X + BIRD_WIDTH >= get<1>(wall) && X - BIRD_WIDTH <= get<1>(wall) + WALL_WIDTH)
		{
			auto aux = get<2>(wall) + ((get<3>(wall) && (score % 2)) ? SLIDE_RATIO : 0);
			if (Y - BIRD_HEIGHT <= WALL_WIDTH * aux)
				end_game = true;

			aux = MAX_GAP - ((get<3>(wall) && (score % 2)) ? SLIDE_HEIGHT : 0);
			if (Y + BIRD_HEIGHT >= aux + WALL_WIDTH * get<2>(wall))
				end_game = true;

			if (end_game)
			{
				cout << "*_* GAME OVER! Final score: " << score << "!\n";
				cout << "\n********************************\n\n";
				return;
			}

			break;
		}
	}

	// --------------------------------------------------------------------------------------------

	model = Translate(X, Y) * Rotate(angle);
	RenderMesh2D(meshes["bird" + to_string(static_cast<int>(wing_pos) % MAX_WING_POS)],
											shaders["VertexColor"], model);

	// --------------------------------------------------------------------------------------------

	if (get<1>(walls.front()) < -WALL_WIDTH)
	{
		walls.pop_front();
		walls.push_back
		({
			generate_wall(score),
			get<1>(walls.back()) + WALL_DISTANCE,
			random_scale(!(score % SLIDING_FREQ)),
			!(score % SLIDING_FREQ)
		});

		FLY_SPEED += FLY_SPEED_INC;
		FALL_SPEED += FALL_SPEED_INC;
		cout << ">>> Your score is " << ++score << " ^_^\n";
	}

	// --------------------------------------------------------------------------------------------

	for (auto& wall: walls)
	{
		get<1>(wall) -= FLY_SPEED;

		if (get<3>(wall) && (score % 2))
		{
			auto aux = get<2>(wall) + SLIDE_RATIO;
			model = Translate(get<1>(wall), 0) * Scale(1, aux);
			RenderMesh2D(get<0>(wall), shaders["VertexColor"], model);
			aux = get<2>(wall) - SLIDE_RATIO;
			model = Translate(get<1>(wall), MAX_GAP + WALL_WIDTH * aux) * Scale(1, WALL_SCALE);
		}
		else
		{
			model = Translate(get<1>(wall), 0) * Scale(1, get<2>(wall));
			RenderMesh2D(get<0>(wall), shaders["VertexColor"], model);
			model = Translate(get<1>(wall), MAX_GAP + WALL_WIDTH * get<2>(wall)) * Scale(1, WALL_SCALE);
		}

		RenderMesh2D(get<0>(wall), shaders["VertexColor"], model);
	}

	RenderMesh2D(meshes["sun"], shaders["VertexColor"], mat4(1));
	RenderMesh2D(meshes["cloud"], shaders["VertexColor"], mat4(1));
}

// ================================================================================================

void Tema1::OnKeyPress(int key, int mods)
{
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_SPACE))
	{
		Y += JUMP_HEIGHT;
		angle = FALL_ANGLE_INC;
	}
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_R))
	{
		walls.clear();
		this->Init();
	}
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_P))
		pause = !pause;
}

// ================================================================================================
