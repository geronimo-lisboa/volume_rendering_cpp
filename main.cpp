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
	bool useFramebuffer = false;
	tela.SetInitCallback([&camera, &vr, &fb]()
	{

		camera = make_shared<Camera>();
		array<float, 3> pos = { {-3.0, -2.0, -5.0} };
		array<float, 3> focus = { {0,0,0} };
		array<float, 3> vUp = { {0,1,0} };
		camera->LookAt(pos, focus, vUp);

		vr = make_shared<VolumeRenderer>();

		fb = make_shared<Framebuffer>();
	});

	tela.SetRenderCallback([&camera, &vr, &fb, &useFramebuffer]()
	{
		if (!useFramebuffer)
		{
			vr->Render(camera);
		}
		else{
			fb->Ativar();
			vr->Render(camera);
			fb->Desativar();
			fb->Render(camera);
		}
	});
	double aX = 0, aY = 0;
	tela.SetOnKeyInputCallback([&useFramebuffer, &aX, &aY, &vr](GLFWwindow*wnd, int key, int scancode, int action, int mods){
		if ((key == GLFW_KEY_F) && (action ==1))
		{

			useFramebuffer = !useFramebuffer;
			cout << "switch do framebuffer" << endl;
		}
		if ((key == GLFW_KEY_RIGHT) && (action == 1))
		{
			array<double, 3> vector = { { 0, 1, 0 } };
			vr->Rotate(vector, --aY);
		}
		if ((key == GLFW_KEY_LEFT) && (action == 1))
		{
			array<double, 3> vector = { { 0, 1, 0 } };
			vr->Rotate(vector, ++aY);
		}
		if ((key == GLFW_KEY_UP) && (action == 1))
		{
			array<double, 3> vector = { { 1, 0, 0 } };
			vr->Rotate(vector, ++aX);
		}
		if ((key == GLFW_KEY_DOWN) && (action == 1))
		{
			array<double, 3> vector = { { 1, 0, 0 } };
			vr->Rotate(vector, --aX);
		}
	});
	tela.InitRenderLoop();
	return 0;
}