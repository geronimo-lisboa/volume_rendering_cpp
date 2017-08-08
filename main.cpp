#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include <array>
#include "camera.h"
#include "tela.h"
using namespace std;

int main(int argc, char** argv)
{
	shared_ptr<Camera> camera = nullptr;
	TelaGLFW tela(500, 500);
	tela.SetInitCallback([&camera]()
	{
		camera = make_shared<Camera>();
		array<float, 3> pos = { {3.0, 2.0, -5.0} };
		array<float, 3> focus = { {0,0,0} };
		array<float, 3> vUp = { {0,1,0} };
		camera->LookAt(pos, focus, vUp);
	});
	tela.SetRenderCallback([&camera]()
	{
		cout << "foo";
	});
	tela.InitRenderLoop();
	return 0;
}