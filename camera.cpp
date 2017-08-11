#include "camera.h"
#include <itkMatrix.h>
#include <itkVector.h>
Camera::Camera()
{
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	view = glm::mat4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);//glm::lookAt()
}
void Camera::LookAt(std::array<float, 3> eye, std::array<float, 3> focus, std::array<float, 3> vup)
{
	this->eye = eye;
	this->focus = focus;
	this->vUp = vup;
	view = glm::lookAt(
		glm::vec3(eye[0], eye[1], eye[2]),
		glm::vec3(focus[0], focus[1], focus[2]),
		glm::vec3(vup[0], vup[1], vup[2])
	);
}
glm::mat4 Camera::GetProjection()
{
	return projection;
}
glm::mat4 Camera::GetView()
{
	return view;
}