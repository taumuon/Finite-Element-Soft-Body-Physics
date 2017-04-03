#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <memory>
#include <vector>

#include "TetrahedralElementIndices.h"
#include "Mesh.h"

class TetrahedralMesh
{
public:
	TetrahedralMesh();
	~TetrahedralMesh();

	Mesh GetMesh();

	std::vector<glm::vec3> GetNodes() { return m_nodes; }
	std::vector<TetrahedralElementIndices> GetElements(){ return m_elements; }
	std::vector<int> GetFixedNodes() { return m_fixed_nodes; }

	void SetNodePosition(int index, glm::vec3 position);

private:
	std::vector<TetrahedralElementIndices> GetTriangularPrismFromNodes(std::vector<int> node_indices);

	std::vector<int> m_fixed_nodes;
	std::vector<glm::vec3> m_nodes;
	std::vector<TetrahedralElementIndices> m_elements;
};

