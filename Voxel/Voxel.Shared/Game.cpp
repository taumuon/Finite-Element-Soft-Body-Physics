#include "pch.h"
#include "Game.h"

#include "CubeMeshFactory.h"
#include "RenderMesh.h"
#include "DrawableMesh.h"
#include "PlaneMeshFactory.h"
#include "ShaderProgram.h"

#include <glm/gtc/quaternion.hpp> // angleaxis

#include <memory>

Game::Game() :
	m_shader_program(std::make_shared<ShaderProgram>())
{
	// doesn't work if building for Android
	// _simpleRenderer = std::make_unique<SimpleRenderer>(_shaderProgram);
	m_simple_renderer = std::unique_ptr<SimpleRenderer>(new SimpleRenderer(m_shader_program));

	CubeMeshFactory cube_factory;
	auto cube_mesh = cube_factory.GetMesh(m_shader_program);
	auto cube_render_mesh = std::make_shared<RenderMesh>(cube_mesh, m_shader_program, true);

	m_cube_drawable_mesh1 = std::make_shared<DrawableMesh>(cube_render_mesh);
	auto initial_transform_cube1 = glm::translate(glm::mat4(), glm::vec3(-1.5, 1.0, -2.0));
	m_cube_drawable_mesh1->SetModelMatrix(initial_transform_cube1);
	// _simpleRenderer->AddMesh(_cubeDrawableMesh1);

	m_cube_drawable_mesh2 = std::make_shared<DrawableMesh>(cube_render_mesh);
	auto initial_transform_cube2 = glm::translate(glm::mat4(), glm::vec3(1.5, 1.0, -2.0));
	m_cube_drawable_mesh2->SetModelMatrix(initial_transform_cube2);
	// _simpleRenderer->AddMesh(_cubeDrawableMesh2);

	PlaneMeshFactory plane;
	auto plane_mesh = plane.GetMesh();
	auto plane_render_mesh = std::make_shared<RenderMesh>(plane_mesh, m_shader_program, true);
	auto plane_drawable_mesh = std::make_shared<DrawableMesh>(plane_render_mesh);
	auto initial_plane_transform = glm::translate(glm::mat4(), glm::vec3(0.0, 0, -2.0));
	plane_drawable_mesh->SetModelMatrix(initial_plane_transform);
	m_simple_renderer->AddMesh(plane_drawable_mesh);

	m_tetrahedral_mesh = std::make_shared<TetrahedralMesh>();
	m_fea = std::make_shared<FEA>(m_tetrahedral_mesh);

	// (x * (yDim * zDim)) + (y * zDim) + z;
	auto force_node_index = (8 * 10) + (4 * 2) + 0;
	m_fea->SetForce(force_node_index, glm::vec3(0.002f, -0.04f, 0.08f));

	auto tetrahedral_mesh_data = m_tetrahedral_mesh->GetMesh();
	auto tetrahedral_render_mesh = std::make_shared<RenderMesh>(tetrahedral_mesh_data, m_shader_program, false);
	m_tetrahedral_drawable_mesh = std::make_shared<DrawableMesh>(tetrahedral_render_mesh);
	m_simple_renderer->AddMesh(m_tetrahedral_drawable_mesh);
}

Game::~Game()
{
}

void Game::Update()
{
	auto mesh1Transform = glm::translate(glm::mat4(), glm::vec3(-1.5, 0.0, -2.0));
	auto mesh2Transform = glm::translate(glm::mat4(), glm::vec3(1.5, 0.0, -2.0));

	// auto radians = 3.14f + (_counter++ / 50.0f) / 200.0f;
	auto radians = 3.65f;
	// auto radians = 1.5f;

	auto rotation_quat1 = glm::angleAxis(radians, glm::vec3(1.0, 0.0, 0.0));
	auto rotation_matrix1 = glm::mat4_cast(rotation_quat1);

	//auto modelMatrix = mMesh1->GetModelMatrix();
	//modelMatrix = rotationMatrix * modelMatrix;
	//mMesh1->SetModelMatrix(modelMatrix);
	m_cube_drawable_mesh1->SetModelMatrix(mesh1Transform * rotation_matrix1);

	auto rotation_quat2 = glm::angleAxis(radians, glm::vec3(0.0, 1.0, 0.0));
	auto rotation_matrix2 = glm::mat4_cast(rotation_quat2);

	//auto modelMatrix2 = mMesh2->GetModelMatrix();
	//modelMatrix2 = rotationMatrix2 * modelMatrix2;
	//mMesh2->SetModelMatrix(modelMatrix2);
	m_cube_drawable_mesh2->SetModelMatrix(mesh2Transform * rotation_matrix2);

	auto rotation_quat3 = glm::angleAxis(radians, glm::vec3(0.0, 1.0, 0.0));
	auto rotation_matrix3 = glm::mat4_cast(rotation_quat3);

	auto mesh3_transform = glm::translate(glm::mat4(), glm::vec3(0.0, 0.0, -1.5));
	m_tetrahedral_drawable_mesh->SetModelMatrix(mesh3_transform * rotation_matrix3);

	m_fea->Update();
	auto new_tetrahedral_mesh = m_tetrahedral_mesh->GetMesh();

	m_tetrahedral_drawable_mesh->UpdateMesh(new_tetrahedral_mesh);
}

void Game::Draw()
{
	m_simple_renderer->Draw();
}

void Game::UpdateWindowSize(GLsizei width, GLsizei height)
{
	m_simple_renderer->UpdateWindowSize(width, height);
}
