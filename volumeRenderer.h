#pragma once
#include "shader.h"
#include <memory>
class VolumeRenderer
{
private:
	std::unique_ptr<Shader> shader;
};