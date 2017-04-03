#include "pch.h"

#include <vector>

#include "CubeMeshFactory.h"
#include "RenderMesh.h"

CubeMeshFactory::CubeMeshFactory()
{
}

Mesh CubeMeshFactory::GetMesh(std::shared_ptr<ShaderProgram> shader_program)
{
	// duplicate points as have normals perpendicular to each face, not interpolated
	std::vector<glm::vec3> m_vertex_positions
	{
		// Bottom
		glm::vec3{ -1.0f, -1.0f, -1.0f },
		glm::vec3{ 1.0f, -1.0f, -1.0f },
		glm::vec3{ -1.0f, -1.0f,  1.0f },
		glm::vec3{ 1.0f, -1.0f,  1.0f },

		// Top
		glm::vec3{ -1.0f, 1.0f, -1.0f },
		glm::vec3{ -1.0f, 1.0f,  1.0f },
		glm::vec3{ 1.0f, 1.0f, -1.0f },
		glm::vec3{ 1.0f, 1.0f,  1.0f },

		// Left
		glm::vec3{ -1.0f, 1.0f, -1.0f },
		glm::vec3{ -1.0f, -1.0f, -1.0f },
		glm::vec3{ -1.0f, 1.0f,  1.0f },
		glm::vec3{ -1.0f, -1.0f,  1.0f },

		// Right
		glm::vec3{ 1.0f, 1.0f, -1.0f },
		glm::vec3{ 1.0f, 1.0f,  1.0f },
		glm::vec3{ 1.0f, -1.0f, -1.0f },
		glm::vec3{ 1.0f, -1.0f,  1.0f },

		// Front
		glm::vec3{ 1.0f, -1.0f,  1.0f },
		glm::vec3{ 1.0f, 1.0f,  1.0f },
		glm::vec3{ -1.0f, -1.0f, 1.0f },
		glm::vec3{ -1.0f, 1.0f, 1.0f },

		// Back
		glm::vec3{ -1.0f, 1.0f, -1.0f },
		glm::vec3{ 1.0f, 1.0f,  -1.0f },
		glm::vec3{ -1.0f, -1.0f, -1.0f },
		glm::vec3{ 1.0f, -1.0f,  -1.0f },
	};

	auto purple = glm::vec3{ 1.0f, 0.0f, 1.0f };
	auto green_blue = glm::vec3{ 0.0f, 1.0f, 1.0f };
	auto green = glm::vec3{ 0.0f, 1.0f, 0.0f };
	auto red = glm::vec3{ 1.0f, 0.0f, 0.0f };
	auto yellow = glm::vec3{ 1.0f, 1.0f, 0.0f };
	auto blue = glm::vec3{ 0.0f, 0.0f, 1.0f };

	std::vector<glm::vec3> m_vertex_colors =
	{
		// bottom
		purple, purple, purple, purple,

		// top
		green_blue, green_blue, green_blue, green_blue,

		// left
		green, green, green, green,

		// right
		red, red, red, red,

		// front
		yellow, yellow, yellow, yellow,

		// back
		blue, blue, blue, blue
	};

	// duplicate points as have normals perpendicular to each face, not interpolated
	std::vector<glm::vec3> m_vertex_normals
	{
		// Bottom
		glm::vec3{ 0.0f, -1.0f, 0.0f },
		glm::vec3{ 0.0f, -1.0f, 0.0f },
		glm::vec3{ 0.0f, -1.0f, 0.0f },
		glm::vec3{ 0.0f, -1.0f, 0.0f },

		// Top
		glm::vec3{ 0.0f, 1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f },

		// Left
		glm::vec3{ -1.0f, 0.0f, 0.0f },
		glm::vec3{ -1.0f, 0.0f, 0.0f },
		glm::vec3{ -1.0f, 0.0f, 0.0f },
		glm::vec3{ -1.0f, 0.0f, 0.0f },

		// Right
		glm::vec3{ 1.0f, 0.0f, 0.0f },
		glm::vec3{ 1.0f, 0.0f, 0.0f },
		glm::vec3{ 1.0f, 0.0f, 0.0f },
		glm::vec3{ 1.0f, 0.0f, 0.0f },

		// Front
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },

		// Back
		glm::vec3{ 0.0f, 0.0f, -1.0f },
		glm::vec3{ 0.0f, 0.0f, -1.0f },
		glm::vec3{ 0.0f, 0.0f, -1.0f },
		glm::vec3{ 0.0f, 0.0f, -1.0f },
	};

	std::vector<short> m_indices =
	{
		// bottom
		0, 1, 2,
		1, 3, 2,

		// top
		4, 5, 6,
		5, 7, 6,

		// left
		8, 9, 10,
		9, 11, 10,

		// right
		12, 13, 14,
		13, 15, 14,

		// front
		16, 17, 18,
		17, 19, 18,

		// back
		20, 21, 22,
		21, 23, 22,
	};

	return Mesh(m_vertex_positions, m_vertex_colors, m_vertex_normals, m_indices);
}

Mesh CubeMeshFactory::GetLineMesh(std::shared_ptr<ShaderProgram> shader_program)
{
	// duplicate points as have normals perpendicular to each face, not interpolated
	std::vector<glm::vec3> m_vertex_positions
	{
		// Bottom
		glm::vec3{ -1.0f, -1.0f, -1.0f },
		glm::vec3{ 1.0f, -1.0f, -1.0f },
		glm::vec3{ -1.0f, -1.0f,  1.0f },
		glm::vec3{ 1.0f, -1.0f,  1.0f },

		// Top
		glm::vec3{ -1.0f, 1.0f, -1.0f },
		glm::vec3{ -1.0f, 1.0f,  1.0f },
		glm::vec3{ 1.0f, 1.0f, -1.0f },
		glm::vec3{ 1.0f, 1.0f,  1.0f },

		// Left
		glm::vec3{ -1.0f, 1.0f, -1.0f },
		glm::vec3{ -1.0f, -1.0f, -1.0f },
		glm::vec3{ -1.0f, 1.0f,  1.0f },
		glm::vec3{ -1.0f, -1.0f,  1.0f },

		// Right
		glm::vec3{ 1.0f, 1.0f, -1.0f },
		glm::vec3{ 1.0f, 1.0f,  1.0f },
		glm::vec3{ 1.0f, -1.0f, -1.0f },
		glm::vec3{ 1.0f, -1.0f,  1.0f },

		// Front
		glm::vec3{ 1.0f, -1.0f,  1.0f },
		glm::vec3{ 1.0f, 1.0f,  1.0f },
		glm::vec3{ -1.0f, -1.0f, 1.0f },
		glm::vec3{ -1.0f, 1.0f, 1.0f },

		// Back
		glm::vec3{ -1.0f, 1.0f, -1.0f },
		glm::vec3{ 1.0f, 1.0f,  -1.0f },
		glm::vec3{ -1.0f, -1.0f, -1.0f },
		glm::vec3{ 1.0f, -1.0f,  -1.0f },
	};

	std::vector<glm::vec3> m_vertex_colors =
	{
		// bottom
		glm::vec3{ 1.0f, 0.0f, 1.0f },
		glm::vec3{ 1.0f, 0.0f, 1.0f },
		glm::vec3{ 1.0f, 0.0f, 1.0f },
		glm::vec3{ 1.0f, 0.0f, 1.0f },

		// top
		glm::vec3{ 0.0f, 1.0f, 1.0f },
		glm::vec3{ 0.0f, 1.0f, 1.0f },
		glm::vec3{ 0.0f, 1.0f, 1.0f },
		glm::vec3{ 0.0f, 1.0f, 1.0f },

		// left
		glm::vec3{ 0.0f, 1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f },

		// right
		glm::vec3{ 1.0f, 0.0f, 0.0f },
		glm::vec3{ 1.0f, 0.0f, 0.0f },
		glm::vec3{ 1.0f, 0.0f, 0.0f },
		glm::vec3{ 1.0f, 0.0f, 0.0f },

		// front
		glm::vec3{ 1.0f, 1.0f, 0.0f },
		glm::vec3{ 1.0f, 1.0f, 0.0f },
		glm::vec3{ 1.0f, 1.0f, 0.0f },
		glm::vec3{ 1.0f, 1.0f, 0.0f },

		// back
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },
	};

	// duplicate points as have normals perpendicular to each face, not interpolated
	std::vector<glm::vec3> m_vertex_normals
	{
		// Bottom
		glm::vec3{ 0.0f, -1.0f, 0.0f },
		glm::vec3{ 0.0f, -1.0f, 0.0f },
		glm::vec3{ 0.0f, -1.0f, 0.0f },
		glm::vec3{ 0.0f, -1.0f, 0.0f },

		// Top
		glm::vec3{ 0.0f, 1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f },
		glm::vec3{ 0.0f, 1.0f, 0.0f },

		// Left
		glm::vec3{ -1.0f, 0.0f, 0.0f },
		glm::vec3{ -1.0f, 0.0f, 0.0f },
		glm::vec3{ -1.0f, 0.0f, 0.0f },
		glm::vec3{ -1.0f, 0.0f, 0.0f },

		// Right
		glm::vec3{ 1.0f, 0.0f, 0.0f },
		glm::vec3{ 1.0f, 0.0f, 0.0f },
		glm::vec3{ 1.0f, 0.0f, 0.0f },
		glm::vec3{ 1.0f, 0.0f, 0.0f },

		// Front
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },
		glm::vec3{ 0.0f, 0.0f, 1.0f },

		// Back
		glm::vec3{ 0.0f, 0.0f, -1.0f },
		glm::vec3{ 0.0f, 0.0f, -1.0f },
		glm::vec3{ 0.0f, 0.0f, -1.0f },
		glm::vec3{ 0.0f, 0.0f, -1.0f },
	};

	std::vector<short> m_indices =
	{
		// bottom
		0, 1, 1, 2, 2, 0,
		1, 3, 3, 2, 2, 1,

		// top
		4, 5, 5, 6, 6, 4,
		5, 7, 7, 6, 6, 5,

		// left
		8, 9, 9, 10, 10, 8,
		9, 11, 11, 10, 10, 9,

		// right
		12, 13, 13, 14, 14, 12,
		13, 15, 15, 14, 14, 13,

		// front
		16, 17, 17, 18, 18, 16,
		17, 19, 19, 18, 18, 17,

		// back
		20, 21, 21, 22, 22, 20,
		21, 23, 23, 22, 22, 21,
	};

	return Mesh(m_vertex_positions, m_vertex_colors, m_vertex_normals, m_indices);
}


CubeMeshFactory::~CubeMeshFactory()
{
}