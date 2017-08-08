#pragma once
#include <array>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
//Classe da câmera.
class Camera
{
private: 
	std::array<float, 3> eye, focus, vUp;
	glm::mat4 projection, view;
public:
	//Cria a câmera com uma matriz de projeção de fovY = 45, planos=[0.01, 200] e aspect = 1. A view
	//tb é criada mas é criada como identidade
	Camera();
	//Configura a matriz de view para ela olhar para um lugar.
	void LookAt(std::array<float, 3> eye, std::array<float, 3> focus, std::array<float, 3> vup);
	glm::mat4 GetProjection();
	glm::mat4  GetView();
};