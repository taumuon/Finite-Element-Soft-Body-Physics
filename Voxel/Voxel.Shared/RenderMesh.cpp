#include "pch.h"
#include "RenderMesh.h"

#include <iostream>

#include "ShaderProgram.h"	

RenderMesh::RenderMesh(Mesh mesh,
					   std::shared_ptr<ShaderProgram> shader_program,
					   bool is_static)
	: m_vertex_position_buffer(0),
	  m_vertex_color_buffer(0),
	  m_vertex_normals_buffer(0),
	  m_index_buffer(0),
	  m_index_count(0),
	  m_shader_program(shader_program)
{
	glGenBuffers(1, &m_vertex_position_buffer);
	glGenBuffers(1, &m_vertex_color_buffer);
	glGenBuffers(1, &m_vertex_normals_buffer);
	glGenBuffers(1, &m_index_buffer);

	UpdateMesh(mesh, is_static);
}

RenderMesh::~RenderMesh()
{
	if (m_vertex_position_buffer != 0)
	{
		glDeleteBuffers(1, &m_vertex_position_buffer);
		m_vertex_position_buffer = 0;
	}

	if (m_vertex_color_buffer != 0)
	{
		glDeleteBuffers(1, &m_vertex_color_buffer);
		m_vertex_color_buffer = 0;
	}

	if (m_vertex_normals_buffer != 0)
	{
		glDeleteBuffers(1, &m_vertex_normals_buffer);
		m_vertex_normals_buffer = 0;
	}

	if (m_index_buffer != 0)
	{
		glDeleteBuffers(1, &m_index_buffer);
		m_index_buffer = 0;
	}
}

void RenderMesh::Draw(glm::mat4 model, GLuint position_attrib_location, GLuint color_attrib_location, GLuint normals_attrib_location)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_position_buffer);
	glEnableVertexAttribArray(position_attrib_location);
	glVertexAttribPointer(position_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_color_buffer);
	glEnableVertexAttribArray(color_attrib_location);
	glVertexAttribPointer(color_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_normals_buffer);
	glEnableVertexAttribArray(normals_attrib_location);
	glVertexAttribPointer(normals_attrib_location, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);

	// if change type to GL_UNSIGNED_INT need to change glBufferData
	glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_SHORT, 0);
	//glLineWidth(5.0);
	//glDrawElements(GL_LINES, _indexCount, GL_UNSIGNED_SHORT, 0);
}

void RenderMesh::UpdateMesh(Mesh mesh, bool is_static)
{
	auto vertex_positions_flattened_data = &mesh.m_vertex_positions[0].x;
	auto vertex_colors_flattened_data = &mesh.m_vertex_colors[0].x;
	auto vertex_normals_flattened_data = &mesh.m_vertex_normals[0].x;

	m_index_count = static_cast<GLsizei>(mesh.m_indices.size());

	auto mode = is_static ? GL_STATIC_DRAW : GL_STREAM_DRAW;

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.m_vertex_positions.size() * 3 * sizeof(GLfloat), vertex_positions_flattened_data, mode);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.m_vertex_colors.size() * 3 * sizeof(GLfloat), vertex_colors_flattened_data, mode);

	glBindBuffer(GL_ARRAY_BUFFER, m_vertex_normals_buffer);
	glBufferData(GL_ARRAY_BUFFER, mesh.m_vertex_normals.size() * 3 * sizeof(GLfloat), vertex_normals_flattened_data, mode);

	// If change from GLushort, need to change glDrawElements to match
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.m_indices.size() * sizeof(GLushort), mesh.m_indices.data(), mode);
}