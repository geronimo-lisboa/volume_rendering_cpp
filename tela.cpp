#include "tela.h"
#include <sstream>
TelaGLFW* TelaGLFW::handleForGLFWCallbacks;
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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
	onKeyInputFunction = nullptr;
	handleForGLFWCallbacks = this;//GAMBIARRA PQ A GLFW USA CALLBACKS DE C E NÃO STD::FUNCTION
	glfwSetKeyCallback(window, TelaGLFW::InternalOnKeyInputCbk);

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

}
void TelaGLFW::SetOnKeyInputCallback(std::function<void(GLFWwindow* wnd, int key, int scancode, int action, int mods)> fn)
{
	this->onKeyInputFunction = fn;
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