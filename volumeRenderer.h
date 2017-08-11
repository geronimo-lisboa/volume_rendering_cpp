#pragma once
#include <gl/glew.h>
#include "shader.h"
#include <memory>
#include "camera.h"
#include <vector>
#include <sstream>
class VolumeRenderer
{
private:
	glm::mat4x4 modelMatrix;
	GLuint vertexArrayObject, vertexBufferObject, vertexColorObject;
	std::vector<GLfloat> vertexes, colors;
	std::shared_ptr<Shader> shader;
	template <typename T> const GLuint CreateBuffer(GLenum target, std::vector<T> &vec);
public:
	VolumeRenderer();
	void Render(const shared_ptr<Camera>& camera);
	void Rotate(array<double, 3> vector, double angInDegs);
};
