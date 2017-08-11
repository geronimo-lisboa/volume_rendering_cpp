#include "volumeRenderer.h"

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
VolumeRenderer::VolumeRenderer()
{
	modelMatrix = glm::mat4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	//Criação do shader
	std::stringstream vsSrc, fsSrc;
	vsSrc << "#version 400" << std::endl;
	vsSrc << "layout(location = 0) in vec3 position;" << std::endl;
	vsSrc << "layout(location = 1) in vec3 color;" << std::endl;
	vsSrc << "uniform mat4 projectionMat;" << std::endl;
	vsSrc << "uniform mat4 viewMat;" << std::endl;
	vsSrc << "uniform mat4 modelMat;" << std::endl;
	vsSrc << "out vec3 vertColor;" << std::endl;
	vsSrc << "	void main() {" << std::endl;
	vsSrc << "	gl_Position = projectionMat * viewMat * modelMat * vec4(position, 1.0);" << std::endl;
	vsSrc << "	vertColor = color;" << std::endl;
	vsSrc << "}" << std::endl;

	fsSrc<<"#version 400" << std::endl;
	fsSrc << "in vec3 vertColor;" << std::endl;
	fsSrc << "out vec4 fragColor;" << std::endl;
	fsSrc << "	void main() {" << std::endl;
	fsSrc << "	fragColor = vec4(vertColor, 1.0);" << std::endl;
	fsSrc << "}" << std::endl;

	shader = std::make_shared<Shader>(vsSrc, fsSrc);
	teste_opengl();
	//O VERTEX ARRAY OBJECT TEM QUE ESTAR CRIADO E BINDADO ANTES DE QQER
	//OPERAÇÃO RELATIVA A VERTICES, COMO glEnableVertexAttribArray. SE O
	//VAO NÃO ESTIVER BINDADO ANTES, VAI DAR ERRO 1282 em glEnableVertexAttribArray.
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
	//Criação da geometria
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(0.1f); colors.push_back(0.0f);

	vertexBufferObject = CreateBuffer<float>(GL_ARRAY_BUFFER, vertexes);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	vertexColorObject = CreateBuffer<float>(GL_ARRAY_BUFFER, colors);
	glBindBuffer(GL_ARRAY_BUFFER, vertexColorObject);
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, 0, NULL);
}

void VolumeRenderer::Render(const shared_ptr<Camera>& camera)
{
	shader->UseProgram();

	glBindVertexArray(vertexArrayObject);

	GLuint positionLocation = shader->GetAttribute("position");
	GLuint colorLocation = shader->GetAttribute("color");
	GLuint modelMatLocation = shader->GetUniform("modelMat");
	GLuint viewMatLocation = shader->GetUniform("viewMat");
	GLuint projectionMatLocation = shader->GetUniform("projectionMat");

	glBindAttribLocation(shader->GetProgramId(), positionLocation, "position");
	glBindAttribLocation(shader->GetProgramId(), colorLocation, "color");
	glm::mat4 proj = camera->GetProjection();
	glm::mat4 view = camera->GetView();
	glUniformMatrix4fv(modelMatLocation, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniformMatrix4fv(viewMatLocation, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projectionMatLocation, 1, GL_FALSE, &proj[0][0]);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}


template <typename T>const GLuint VolumeRenderer::CreateBuffer(GLenum target, std::vector<T> &vec)
{
	GLuint resultBuffer = 0;
	glGenBuffers(1, &resultBuffer);
	glBindBuffer(target, resultBuffer);
	glBufferData(target, vec.size() * sizeof(vec.at(0)), vec.data(), GL_STATIC_DRAW);
	return resultBuffer;
}