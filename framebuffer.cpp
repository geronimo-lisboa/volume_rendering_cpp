#include <iostream>
#include "framebuffer.h"

void Framebuffer::Ativar()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

void Framebuffer::Desativar()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &framebuffer);
	glGenRenderbuffers(1, &depthRenderbuffer);
	glGenTextures(1, &texture);
	//Cria e configura a textura
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 500, 500, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//Cria e configura o renderbuffer
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 500, 500);
	//Bind do framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	//atacha a textura como color attachment
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, texture, 0);
	//e o depth buffer como depth attachment
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
	GLenum statusDoFramebuffer = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (statusDoFramebuffer != GL_FRAMEBUFFER_COMPLETE)
	{
		std::stringstream erroFb;
		erroFb << "ERRO DE FRAMEBUFFER CODIGO = " << statusDoFramebuffer;
		throw std::exception(erroFb.str().c_str());
	}
	std::cout << "Aparentemente o framebuffer foi criado com sucesso" << std:: endl;
	//Cria��o do shader
	std::stringstream vsSrc, fsSrc;
	vsSrc << "#version 400" << std::endl;
	vsSrc << "layout(location = 0) in vec3 position;" << std::endl;
	vsSrc << "layout(location = 1) in vec3 color;" << std::endl;
	vsSrc << "out vec3 vertColor;" << std::endl;
	vsSrc << "	void main() {" << std::endl;
	vsSrc << "	gl_Position = vec4(position, 1.0);" << std::endl;
	vsSrc << "	vertColor = color;" << std::endl;
	vsSrc << "}" << std::endl;

	fsSrc << "#version 400" << std::endl;
	fsSrc << "in vec3 vertColor;" << std::endl;
	fsSrc << "uniform sampler2D renderedTexture;" << std::endl;
	fsSrc << "out vec4 fragColor;" << std::endl;
	fsSrc << "	void main() {" << std::endl;
	fsSrc << "  vec2 tc = vec2(vertColor[0], vertColor[1]);" << std::endl;
	fsSrc << "	fragColor = texture(renderedTexture, tc);" << std::endl;
	fsSrc << "	fragColor[0]=0.0;" << std::endl;
	fsSrc << "}" << std::endl;

	shader = std::make_shared<Shader>(vsSrc, fsSrc);
	teste_opengl();
	//O VERTEX ARRAY OBJECT TEM QUE ESTAR CRIADO E BINDADO ANTES DE QQER
	//OPERA��O RELATIVA A VERTICES, COMO glEnableVertexAttribArray. SE O
	//VAO N�O ESTIVER BINDADO ANTES, VAI DAR ERRO 1282 em glEnableVertexAttribArray.
	vertexArrayObject = 0;
	glGenVertexArrays(1, &vertexArrayObject);
	glBindVertexArray(vertexArrayObject);
	//Ativa os locais dos atributos no shader
	shader->UseProgram();
	GLuint positionLocation = shader->GetAttribute("position");
	GLuint colorLocation = shader->GetAttribute("color");
	glEnableVertexAttribArray(positionLocation);
	glEnableVertexAttribArray(colorLocation);
	glUseProgram(0);
	//Cria��o da geometria
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(0.0f);

	vertexBufferObject = CreateBuffer<float>(GL_ARRAY_BUFFER, vertexes);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	vertexColorObject = CreateBuffer<float>(GL_ARRAY_BUFFER, colors);
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorObject);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Render(const shared_ptr<Camera>& camera)
{
	shader->UseProgram();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(vertexArrayObject);
	GLuint texID = shader->GetUniform("renderedTexture");
	GLuint positionLocation = shader->GetAttribute("position");
	GLuint colorLocation = shader->GetAttribute("color");

	glBindAttribLocation(shader->GetProgramId(), positionLocation, "position");
	glBindAttribLocation(shader->GetProgramId(), colorLocation, "color");
	glUniform1i(texID, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


template <typename T>const GLuint Framebuffer::CreateBuffer(GLenum target, std::vector<T> &vec)
{
	GLuint resultBuffer = 0;
	glGenBuffers(1, &resultBuffer);
	glBindBuffer(target, resultBuffer);
	glBufferData(target, vec.size() * sizeof(vec.at(0)), vec.data(), GL_STATIC_DRAW);
	return resultBuffer;
}