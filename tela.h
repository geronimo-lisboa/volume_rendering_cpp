#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
/*
 *Isso aqui encapsula a janela da GLFW. O usu�rio deve prover um m�todo para ser executado
 *para inicializa��o da cena, na renderiza��o do primeiro quadro, e um m�todo para ser 
 *executado no Render();
 */
class TelaGLFW
{
private:
	GLFWwindow *window;
	bool isInitialized;
	int screenWidth, screenHeight;
	std::function<void()> initFunction, renderFunction;
public:
	/*Construtor - cria a janela*/
	TelaGLFW(int screenWidth, int screenHeight);
	/*Construtor - destr�i a janela*/
	~TelaGLFW();
	void SetInitCallback(std::function<void()> fn);
	void SetRenderCallback(std::function<void()> fn);
	void InitRenderLoop();
};