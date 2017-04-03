#pragma once

#include "SimpleRenderer.h"

#include "TetrahedralMesh.h"
#include "FEA.h"

class DrawableMesh;
class ShaderProgram;

class Game
{
public:
	Game();
	~Game();
	void UpdateWindowSize(GLsizei width, GLsizei height);
	void Update();
	void Draw();
private:
	std::unique_ptr<SimpleRenderer> m_simple_renderer;
	std::shared_ptr<ShaderProgram> m_shader_program;
	std::shared_ptr<DrawableMesh> m_cube_drawable_mesh1;
	std::shared_ptr<DrawableMesh> m_cube_drawable_mesh2;

	std::shared_ptr<DrawableMesh> m_tetrahedral_drawable_mesh;

	std::shared_ptr<TetrahedralMesh> m_tetrahedral_mesh;
	std::shared_ptr<FEA> m_fea;
};

