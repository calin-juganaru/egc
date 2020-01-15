#include "Laborator1.h"

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <Core/Engine.h>

using namespace std;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	Mesh* mesh1 = new Mesh("clopot");
	mesh1->LoadMesh(RESOURCE_PATH::MODELS, "clopot_bronz.obj");
	meshes[mesh1->GetMeshID()] = mesh1;

	Mesh* mesh2 = new Mesh("box");
	mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh2->GetMeshID()] = mesh2;

	Mesh* mesh3 = new Mesh("sphere");
	mesh3->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh3->GetMeshID()] = mesh3;

	srand(time(nullptr));
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glClearColor(0, 1, 0, 0);
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_F) == GLFW_PRESS)
		glClearColor(1, 0, 0, 0);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	static auto mesh = meshes["box"];
	static auto pos = glm::vec3(0.5f, 0, 0);
	static auto scale = glm::vec3(1);
	static auto jumping = false;

	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_1) == GLFW_PRESS)
	{
		mesh = meshes["clopot"];
		//pos = glm::vec3(-1, 0.5f, 0);
		scale = glm::vec3(0.01f);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_2) == GLFW_PRESS)
	{
		mesh = meshes["sphere"];
		//pos = glm::vec3(0.5f, 2, 1);
		scale = glm::vec3(0.5f);
	}

	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		pos.z -= 0.1;
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		pos.z += 0.1;
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		pos.x -= 0.1;
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		pos.x += 0.1;
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		pos.y -= 0.1;
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_E) == GLFW_PRESS)
	{
		pos.y += 0.1;
	}
	
	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		jumping = true;
	}

	static auto w = 2, h = 2;

	if (jumping)
	{
		pos.x += 0.1;
		pos.y += 0.1;
		
		if (pos.x == w)
			w /= 2;
		
		if (pos.y == h)
			h /= 2;
	}

	RenderMesh(mesh, pos, scale);
}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
};

void Laborator1::OnKeyPress(int key, int mods)
{
	
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	this->OnKeyPress(key, mods);
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
