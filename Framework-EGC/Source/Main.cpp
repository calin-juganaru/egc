#include <ctime>
#include <iostream>

using namespace std;

#include <Core/Engine.h>
#include <Laboratoare/LabList.h>

int main(int argc, char **argv)
{
	srand(time(nullptr));

	WindowProperties wp;
	wp.resolution = glm::ivec2(1280, 720);

	WindowObject* window = Engine::Init(wp);

	World* world = new Laborator9();
	world->Init();
	world->Run();

	Engine::Exit();
}