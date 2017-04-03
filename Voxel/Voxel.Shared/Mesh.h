#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <vector>

struct Mesh
{
public:
	Mesh(std::vector<glm::vec3> vertex_positions,
		std::vector<glm::vec3> vertex_colors,
		std::vector<glm::vec3> vertex_normals,
		std::vector<short> indices)
	{
		m_vertex_positions = vertex_positions;
		m_vertex_colors = vertex_colors;
		m_vertex_normals = vertex_normals;
		m_indices = indices;
	}

	std::vector<glm::vec3> m_vertex_positions;
	std::vector<glm::vec3> m_vertex_colors;
	std::vector<glm::vec3> m_vertex_normals;
	std::vector<short> m_indices;
};

