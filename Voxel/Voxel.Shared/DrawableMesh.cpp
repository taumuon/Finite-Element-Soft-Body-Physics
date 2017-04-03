#include "pch.h"
#include "DrawableMesh.h"
#include "RenderMesh.h"
#include "Mesh.h"

#include <iostream>

DrawableMesh::DrawableMesh(std::shared_ptr<RenderMesh> mesh)
	: m_mesh(mesh),
	  m_model_matrix(glm::mat4())
{
}

DrawableMesh::~DrawableMesh()
{
}


void DrawableMesh::Draw(glm::mat4 view, glm::mat4 proj, GLuint position_attrib_location, GLuint color_attrib_location, GLuint normals_attrib_location)
{
	// TOOD: create mesh with a ShaderProgram object, so don't need to pass in the attribLocations here

	m_mesh->Draw(m_model_matrix, position_attrib_location, color_attrib_location, normals_attrib_location);
}

void DrawableMesh::UpdateMesh(Mesh mesh)
{
	m_mesh->UpdateMesh(mesh);
}