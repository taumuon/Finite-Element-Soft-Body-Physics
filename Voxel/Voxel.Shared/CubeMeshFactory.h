#pragma once

#include <memory>

#include "Mesh.h"

class ShaderProgram;

class CubeMeshFactory
{
public:
	CubeMeshFactory();
	~CubeMeshFactory();

	Mesh GetMesh(std::shared_ptr<ShaderProgram> shader_program);

	Mesh GetLineMesh(std::shared_ptr<ShaderProgram> shader_program);
};

