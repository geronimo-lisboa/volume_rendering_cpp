#pragma once
#include "shader.h"
#include <memory>
#include "camera.h"
class VolumeRenderer
{
private:
	std::unique_ptr<Shader> shader;
public:
	void Render(const shared_ptr<Camera>& camera);
};