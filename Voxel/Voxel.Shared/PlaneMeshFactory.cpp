#include "pch.h"
#include "PlaneMeshFactory.h"
#include "RenderMesh.h"
#include "ShaderProgram.h"

PlaneMeshFactory::PlaneMeshFactory()
{
}


PlaneMeshFactory::~PlaneMeshFactory()
{
}

// Draws a simple ground plane checkerboard, 10 squares in each (+ve and -ve direction).
// Very inefficient, would be better to do this as a shader
Mesh PlaneMeshFactory::GetMesh()
{
	std::vector<glm::vec3> m_vertex_positions;

	std::vector<glm::vec3> m_vertex_normals;

	std::vector<glm::vec3> m_vertex_colors;

	std::vector<short> m_indices;

	for (auto x = -10; x < 10; ++x)
	{
		for (auto z = -10; z < 10; ++z)
		{
			auto start_index = static_cast<int>(m_vertex_positions.size());

			auto point1 = glm::vec3{ x, 0.0f, z };
			auto point2 = glm::vec3{ x + 1, 0.0f, z };
			auto point3 = glm::vec3{ x, 0.0f, z + 1 };
			auto point4 = glm::vec3{ x + 1, 0.0f, z + 1 };

			m_vertex_positions.push_back(point1);
			m_vertex_positions.push_back(point2);
			m_vertex_positions.push_back(point3);
			m_vertex_positions.push_back(point4);

			m_vertex_normals.push_back(glm::vec3{ 0.0f, 1.0f, 0.0f });
			m_vertex_normals.push_back(glm::vec3{ 0.0f, 1.0f, 0.0f });
			m_vertex_normals.push_back(glm::vec3{ 0.0f, 1.0f, 0.0f });
			m_vertex_normals.push_back(glm::vec3{ 0.0f, 1.0f, 0.0f });

			auto is_dark = x % 2 == 0 ? (z % 2 == 0) : (z % 2 != 0);
			auto color = is_dark ? glm::vec3{ 1.0f, 1.0f, 1.0f } : glm::vec3{ 1.0f, 0.0f, 0.0f };

			m_vertex_colors.push_back(color);
			m_vertex_colors.push_back(color);
			m_vertex_colors.push_back(color);
			m_vertex_colors.push_back(color);

			m_indices.push_back(start_index);
			m_indices.push_back(start_index + 2);
			m_indices.push_back(start_index + 1);

			m_indices.push_back(start_index + 1);
			m_indices.push_back(start_index + 2);
			m_indices.push_back(start_index + 3);
		}
	}

	return Mesh(m_vertex_positions, m_vertex_colors, m_vertex_normals, m_indices);
}
