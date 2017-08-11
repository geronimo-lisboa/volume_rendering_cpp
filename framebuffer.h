#pragma once
#include <gl/glew.h>
#include "shader.h"
#include <memory>
#include "camera.h"
#include <vector>
#include <sstream>
class Framebuffer
{
private:
	//Os nomes do framebuffer, do depthRenderBuffer e da textura
	GLuint framebuffer, depthRenderbuffer, texture;
	GLuint vertexArrayObject, vertexBufferObject, vertexColorObject;
	std::vector<GLfloat> vertexes, colors;
	std::shared_ptr<Shader> shader;
	template <typename T> const GLuint CreateBuffer(GLenum target, std::vector<T> &vec);
	void teste_opengl()
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			std::stringstream ss;
			ss << "erro de opengl " << err;
			throw std::exception(ss.str().c_str());
		}
	}
public:
	Framebuffer();
	void Render(const shared_ptr<Camera>& camera);
	void Ativar();
	void Desativar();
};
