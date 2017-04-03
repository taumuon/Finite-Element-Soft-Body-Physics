#pragma once

#include <memory>

#include "Mesh.h"

class ShaderProgram;

class PlaneMeshFactory
{
public:
	PlaneMeshFactory();
	~PlaneMeshFactory();

	Mesh GetMesh();
};

