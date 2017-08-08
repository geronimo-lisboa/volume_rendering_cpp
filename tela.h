#pragma once
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <functional>
/*
 *Isso aqui encapsula a janela da GLFW. O usuário deve prover um método para ser executado
 *para inicialização da cena, na renderização do primeiro quadro, e um método para ser 
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
	/*Construtor - destrói a janela*/
	~TelaGLFW();
	void SetInitCallback(std::function<void()> fn);
	void SetRenderCallback(std::function<void()> fn);
	void InitRenderLoop();
};