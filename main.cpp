#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "tela.h"
int main(int argc, char** argv)
{
	TelaGLFW tela(500, 500);
	tela.SetInitCallback([]()
	{
		
	});
	tela.SetRenderCallback([]()
	{
		
	});
	tela.InitRenderLoop();
	return 0;
}