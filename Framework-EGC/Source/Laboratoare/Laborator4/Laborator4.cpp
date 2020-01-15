#include "Laborator4.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include "Transform3D.h"

using namespace std;

Laborator4::Laborator4()
{
	
}

Laborator4::~Laborator4()
{
}

void Laborator4::Init()
{
	polygonMode = GL_FILL;

	Mesh* mesh = new Mesh("box");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;

	Mesh* mesh2 = new Mesh("sphere");
	mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh2->GetMeshID()] = mesh2;

	// initialize tx, ty and tz (the translation steps)
	translateX = 0;
	translateY = 0;
	translateZ = 0;

	// initialize sx, sy and sz (the scale factors)
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
	
	// initialize angularSteps
	angularStepOX = 0;
	angularStepOY = 0;
	angularStepOZ = 0;
}

void Laborator4::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator4::Update(float deltaTimeSeconds)
{
	glLineWidth(3);
	glPointSize(5);
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	static auto modelMatrix1 = glm::mat4(1) * Transform3D::Translate(-2.5f, 0.5f, -1.5f);
	static auto modelMatrix2 = glm::mat4(1) * Transform3D::Translate(0.0f, 0.5f, -1.5f);
	static auto modelMatrix3 = glm::mat4(1) * Transform3D::Translate(2.5f, 0.5f, -1.5f);

	if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_W) == GLFW_PRESS)
	{
		modelMatrix1 *= Transform3D::Translate(0, deltaTimeSeconds, 0);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_S) == GLFW_PRESS)
	{
		modelMatrix1 *= Transform3D::Translate(0, -deltaTimeSeconds, 0);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_A) == GLFW_PRESS)
	{
		modelMatrix1 *= Transform3D::Translate(-deltaTimeSeconds, 0, 0);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_D) == GLFW_PRESS)
	{
		modelMatrix1 *= Transform3D::Translate(deltaTimeSeconds, 0, 0);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_F) == GLFW_PRESS)
	{
		modelMatrix1 *= Transform3D::Translate(0, 0, deltaTimeSeconds);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_R) == GLFW_PRESS)
	{
		modelMatrix1 *= Transform3D::Translate(0, 0, -deltaTimeSeconds);
	}

	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_1) == GLFW_PRESS)
	{
		modelMatrix2 *= Transform3D::Scale(1.05, 1.05, 1.05);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_2) == GLFW_PRESS)
	{
		modelMatrix2 *= Transform3D::Scale(1 / 1.05, 1 / 1.05, 1 / 1.05);
	}

	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_3) == GLFW_PRESS)
	{
		modelMatrix3 *= Transform3D::RotateOX(deltaTimeSeconds);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_4) == GLFW_PRESS)
	{
		modelMatrix3 *= Transform3D::RotateOX(-deltaTimeSeconds);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_5) == GLFW_PRESS)
	{
		modelMatrix3 *= Transform3D::RotateOY(deltaTimeSeconds);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_6) == GLFW_PRESS)
	{
		modelMatrix3 *= Transform3D::RotateOY(-deltaTimeSeconds);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_7) == GLFW_PRESS)
	{
		modelMatrix3 *= Transform3D::RotateOZ(deltaTimeSeconds);
	}
	else if (glfwGetKey(window->GetGLFWWindow(), GLFW_KEY_8) == GLFW_PRESS)
	{
		modelMatrix3 *= Transform3D::RotateOZ(-deltaTimeSeconds);
	}

	//RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix1);
	//RenderMesh(meshes["sphere"], shaders["Simple"], modelMatrix2);
	//RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix3);

	static auto terra = glm::mat4(1) * Transform3D::Translate(-2.5f, 0.5f, -1.5f)
		* Transform3D::Scale(0.5, 0.5, 0.5);
	static auto soare = glm::mat4(1) * Transform3D::Translate(0.0f, 0.5f, -1.5f);
	static auto luna = glm::mat4(1) * Transform3D::Translate(-2.5f, 0.5f, -1.5f) * Transform3D::Scale(0.25, 0.25, 0.25);

	soare *= Transform3D::RotateOY(deltaTimeSeconds);

	terra *= Transform3D::Translate(5.0f, 0.5f, -1.5f);
	terra *= Transform3D::RotateOY(deltaTimeSeconds);
	terra *= Transform3D::Translate(-5.0f, -0.5f, 1.5f);
	
	luna *= Transform3D::Translate(5.0f, 0.5f, -1.5f);
	luna *= Transform3D::RotateOY(deltaTimeSeconds);
	luna *= Transform3D::Translate(0.0f, 0.5f, -1.5f);
	luna *= Transform3D::RotateOY(deltaTimeSeconds);
	luna *= Transform3D::Translate(0.0f, -0.5f, 1.5f);
	//luna *= Transform3D::RotateOY(deltaTimeSeconds);
	luna *= Transform3D::Translate(-5.0f, -0.5f, 1.5f);

	RenderMesh(meshes["sphere"], shaders["VertexNormal"], terra);
	RenderMesh(meshes["sphere"], shaders["VertexNormal"], soare);
	RenderMesh(meshes["sphere"], shaders["VertexNormal"], luna);
}

void Laborator4::FrameEnd()
{
	DrawCoordinatSystem();
}

void Laborator4::OnInputUpdate(float deltaTime, int mods)
{
}

void Laborator4::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE)
	{
		switch (polygonMode)
		{
		case GL_POINT:
			polygonMode = GL_FILL;
			break;
		case GL_LINE:
			polygonMode = GL_POINT;
			break;
		default:
			polygonMode = GL_LINE;
			break;
		}
	}
}

void Laborator4::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator4::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator4::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator4::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator4::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator4::OnWindowResize(int width, int height)
{
}
