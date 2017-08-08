#pragma once
#include <eigen3/Eigen/Core>
#include<array>
using namespace Eigen;
class Camera
{
private: 
	std::array<float, 3> eye, focus, vUp;
	Matrix<float, 4, 4, DontAlign> projection, view;
public:
	Camera();
	void LookAt(std::array<float, 3> eye, std::array<float, 3> focus, std::array<float, 3> vup);
};