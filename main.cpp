#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include <array>
#include "camera.h"
#include "tela.h"
#include "volumeRenderer.h"
#include "framebuffer.h"
using namespace std;

int main(int argc, char** argv)
{
	shared_ptr<Camera> camera = nullptr;
	shared_ptr<VolumeRenderer> vr = nullptr;
	shared_ptr<Framebuffer> fb = nullptr;
	TelaGLFW tela(500, 500);

	tela.SetInitCallback([&camera, &vr, &fb]()
	{
		//Inicialização da câmera
		camera = make_shared<Camera>();
		array<float, 3> pos = { {3.0, 2.0, -5.0} };
		array<float, 3> focus = { {0,0,0} };
		array<float, 3> vUp = { {0,1,0} };
		camera->LookAt(pos, focus, vUp);
		//Inicialização do vr
		vr = make_shared<VolumeRenderer>();
		//Inicialização do framebuffer;
		fb = make_shared<Framebuffer>();
	});

	tela.SetRenderCallback([&camera, &vr, &fb]()
	{
		vr->Render(camera);
		fb->Render(camera);
	});
	tela.InitRenderLoop();
	return 0;
}