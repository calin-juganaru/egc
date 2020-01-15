#undef UNICODE
#undef _UNICODE

#include <ctime>
#include <iostream>

using namespace std;

#include <Core/Engine.h>
#include <Tema3.h>

int main(int argc, char **argv)
{
	srand(time(nullptr));

	WindowProperties wp;
	wp.resolution = glm::ivec2(1280, 720);

	WindowObject* window = Engine::Init(wp);

	World* world = new Tema3();
	world->Init();
	world->Run();

	Engine::Exit();
}