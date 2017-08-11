#define GLM_ENABLE_EXPERIMENTAL

#include "volumeRenderer.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cctype>
#include <locale>
#include <regex>
#include <array>
#include <ctime>
#include <chrono>
///////ESSAS COISAS AQUI DEVERIAM ESTAR EM UM UTILS.H
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}
static inline  std::vector<std::string> SplitString(const std::string& input, const std::string& regex) {
	// passing -1 as the submatch index parameter performs splitting
	std::regex re(regex);
	std::sregex_token_iterator
		first{ input.begin(), input.end(), re, -1 },
		last;
	return{ first, last };
}
//Current time as string
static inline std::string GetDateAsString()
{
	std::chrono::time_point<std::chrono::system_clock> hora = std::chrono::system_clock::now();
	std::time_t end_time = std::chrono::system_clock::to_time_t(hora);
	std::string t(std::ctime(&end_time));
	t = trim(t);
	return t;
}
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
//---------------------------------------------
void VolumeRenderer::Rotate(array<double, 3> vector, double angInDegs)
{
	const glm::vec3 v = glm::vec3(vector[0], vector[1], vector[2]);
	modelMatrix = glm::rotate(glm::radians<float>(angInDegs), v);
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
	//Leitura do arquivo
	const std::string path_do_cubo = "C:/Nova pasta (2)/volume_rendering_cpp/cubo.obj";
	std::vector<std::string> linhasDoArquivo;
	ifstream myReadFile;
	myReadFile.open(path_do_cubo);
	if (!myReadFile.is_open()) throw std::exception("não foi possivel abrir o arquivo");
	std::string temp;
	while (std::getline(myReadFile,temp))
	{
		linhasDoArquivo.push_back(temp);
	}
	std::vector<std::array<float, 3>> tempVecList;
	std::vector < std::array<unsigned int, 3>> tempTrianglePointsList;
	for (std::string ln : linhasDoArquivo)
	{
		std::vector<std::string> splittedLine = SplitString(ln, " ");
		if (splittedLine[0] == "o")//Objeto
			std::cout << "Nome do objeto = " << splittedLine[1] << std::endl;
		if (splittedLine[0] == "v") //Vertices
		{
			std::array<float, 3> point = { { stof(splittedLine[1]), stof(splittedLine[2]), stof(splittedLine[3]) } };
			tempVecList.push_back(point);
		}
		if (splittedLine[0] == "f")//definição de uma face
		{
			std::vector<std::string> t1 = SplitString(splittedLine[1], "//");
			std::vector<std::string> t2 = SplitString(splittedLine[2], "//");
			std::vector<std::string> t3 = SplitString(splittedLine[2], "//");
			std::array<unsigned int, 3> verts = { {
					stoi(t1[0]), stoi(t2[0]), stoi(t3[0]),
				} };
			tempTrianglePointsList.push_back(verts);
		}
	}
	//Percorre a lista temporária das faces e as adiciona à lista permanente (vertexes)
	for (array<unsigned int, 3> currentTemp : tempTrianglePointsList)
	{
		std::array<float, 3> v0
	}


	//Criação da geometria
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(1.0f);
	//
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(-1.0f);
	//
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(-1.0f);
	//
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(-1.0f);
	//
	vertexes.push_back(1.0f);  vertexes.push_back(1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(1.0f); vertexes.push_back(-1.0f);
	//
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(1.0f);  vertexes.push_back(-1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(-1.0f);
	vertexes.push_back(-1.0f); vertexes.push_back(-1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(1.0f);
	vertexes.push_back(1.0f); vertexes.push_back(-1.0f); vertexes.push_back(-1.0f);

	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(0.0f);
	//
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(0.0f);
	//
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	colors.push_back(1.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	//
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(1.0f); colors.push_back(1.0f);
	//
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(0.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	//
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(1.0f);
	colors.push_back(1.0f); colors.push_back(0.0f); colors.push_back(1.0f);

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

	glDrawArrays(GL_TRIANGLES, 0, vertexes.size()/3);
}


template <typename T>const GLuint VolumeRenderer::CreateBuffer(GLenum target, std::vector<T> &vec)
{
	GLuint resultBuffer = 0;
	glGenBuffers(1, &resultBuffer);
	glBindBuffer(target, resultBuffer);
	glBufferData(target, vec.size() * sizeof(vec.at(0)), vec.data(), GL_STATIC_DRAW);
	return resultBuffer;
}