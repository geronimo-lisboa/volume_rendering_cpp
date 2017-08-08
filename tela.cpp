#include "tela.h"
#include <sstream>

TelaGLFW::TelaGLFW(int screenWidth, int screenHeight)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	//Seta o callback de erro.
	glfwSetErrorCallback([](int error, const char* description) {
		std::stringstream ss;
		ss << "Erro: " << error << " : " << description;
		throw std::exception(ss.str().c_str());
	});
	//Se falhou em iniciar, morre
	if (!glfwInit()) { throw std::exception("Falha no glfwInit(). Tem algo MUITO errado acontecendo"); }
	//OpenGL 3.2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	window = glfwCreateWindow(screenWidth, screenHeight, "BOA NOITE", NULL, NULL);//A cria��o da janela � aqui
	if (!window)//Se falhou em criar a janela, morre.
	{
		glfwTerminate();
		throw std::exception("Nao foi possivel criar a janela");
	}

	glfwMakeContextCurrent(window);
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* A fonte de erro mais comum � tentar iniciar a glew sem ter um contexto opengl funcionando,
		sem ter a tela ativa.*/
		std::stringstream ss;
		ss << "Erro de glew: " << glewGetErrorString(err);
		throw std::exception(ss.str().c_str());
	}
	//O makeCurrent() da glfw
	glfwSwapInterval(1);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	isInitialized = false;
	initFunction = nullptr;
	renderFunction = nullptr;
}

TelaGLFW::~TelaGLFW()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void TelaGLFW::SetInitCallback(std::function<void()> fn)
{
	this->initFunction = fn;
}

void TelaGLFW::SetRenderCallback(std::function<void()> fn)
{
	this->renderFunction = fn;
}

void TelaGLFW::InitRenderLoop()
{
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if(initFunction && !isInitialized)
		{
			initFunction();
			isInitialized = true;
		}
		else if(renderFunction)
		{
			renderFunction();
		}
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
}