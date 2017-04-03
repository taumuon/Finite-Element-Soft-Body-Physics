#include "pch.h"
#include "FEA.h"

#include "TetrahedralMesh.h"

#include "Eigen/Dense"
#include "Eigen/IterativeLinearSolvers"

#include <iostream>
#include <algorithm>
#include <array>

FEA::FEA(std::shared_ptr<TetrahedralMesh> mesh)
	: m_mesh(mesh),
	  m_delta_time(0.0001)
{
	AssembleGlobalStiffnessMatrix(0.5, 0.5);
	CreateFloatingNodeStiffnessMatrix();

	auto nodes = m_mesh->GetNodes();
	auto fixedNodes = m_mesh->GetFixedNodes();
	m_xdim = static_cast<int>((nodes.size() - fixedNodes.size()) * 3);

	auto mass_value = 0.01;
	CreateLumpedMassMatrix(m_xdim, mass_value);

	auto x0 = Eigen::MatrixXd(m_xdim, 1);
	for (auto node_index = 0; node_index < nodes.size(); ++node_index)
	{
		auto target_node_index = m_original_node_indices_mapped_to_non_fixed[node_index];
		if (target_node_index == -1) { continue; }

		auto target_index = target_node_index * 3;

		auto non_fixed_node = nodes[node_index];
		x0(target_index, 0) = non_fixed_node.x;
		x0(target_index + 1, 0) = non_fixed_node.y;
		x0(target_index + 2, 0) = non_fixed_node.z;
	}
	m_x0 = x0;

	m_v = Eigen::MatrixXd(m_xdim, 1);
	m_v.setZero();

	m_x = m_x0;

	m_A = m_lumped_mass_matrix + /*delta_time * damping +*/ (m_delta_time * m_delta_time) * m_floating_node_stiffness_matrix;

	// In case user doesn't set a force
	m_force = Eigen::MatrixXd(m_xdim, 1);
	m_force.setZero();
}

FEA::~FEA()
{
}

void FEA::SetForce(int node_index, glm::vec3 force)
{
	m_force = Eigen::MatrixXd(m_xdim, 1);
	m_force.setZero();

	auto resulting_index = m_original_node_indices_mapped_to_non_fixed[node_index];
	if (resulting_index == -1)
	{
		// Trying to set a force on a fixed node
		return;
	}

	auto force_index = resulting_index * 3;
	m_force(force_index, 0) = force.x;
	m_force(force_index + 1, 0) = force.y;
	m_force(force_index + 2, 0) = force.z;
}


// NOTE: doesn't take delta_time, as if varying will need to remultiply A matrix by delta_time^2.
// TODO: need to add a method to allow delta_time to be set (and then remultiply A matrix)
void FEA::Update()
{
	auto displacement = m_x - m_x0;

	auto Kf = m_floating_node_stiffness_matrix * displacement;

	// Explicitly specify type to avoid auto-template expression problem
	Eigen::MatrixXd B = (m_lumped_mass_matrix * m_v) - (m_delta_time * (Kf - m_force));

	Eigen::ConjugateGradient<Eigen::MatrixXd> cg_inner;
	cg_inner.compute(m_A);
	Eigen::MatrixXd v_new = cg_inner.solve(B);

	m_v = v_new;
	m_x = m_x + (m_delta_time * v_new);

	auto nodes = m_mesh->GetNodes();
	auto fixed_nodes = m_mesh->GetFixedNodes();

	for (auto node_index = 0; node_index < nodes.size(); ++node_index)
	{
		auto target_node_index = m_original_node_indices_mapped_to_non_fixed[node_index];
		if (target_node_index == -1) { continue; }

		auto target_index = target_node_index * 3;

		auto position = glm::vec3(m_x(target_index, 0), m_x(target_index + 1, 0), m_x(target_index + 2, 0));

		m_mesh->SetNodePosition(node_index, position);
	}
}

void FEA::CreateLumpedMassMatrix(int num_nodes, double total_mass)
{
	auto mass = total_mass / num_nodes;

	m_lumped_mass_matrix = Eigen::MatrixXd::Identity(num_nodes, num_nodes) * mass;
}

Eigen::MatrixXd FEA::BuildDampingMatrix(int num_nodes, double damping)
{
	return Eigen::MatrixXd::Identity(num_nodes, num_nodes) * damping;
}

Eigen::Matrix4d FEA::BuildBarycentricMatrix(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec3 x4)
{
	Eigen::Matrix4d Pe;

	Pe.setOnes();

	for (auto i = 1; i < 4; ++i)
	{
		Pe(i, 0) = x1[i - 1];
		Pe(i, 1) = x2[i - 1];
		Pe(i, 2) = x3[i - 1];
		Pe(i, 3) = x4[i - 1];
	}

	return Pe.inverse();
}

// Displacement-deformation matrix
// Pe is barycentric matrix
Eigen::Matrix<double, 6, 12> FEA::BuildBe(Eigen::Matrix4d Pe)
{
	Eigen::Matrix<double, 6, 12> be;

	be.setZero();

	// First row
	be(0, 0) = Pe(0, 0); be(0, 3) = Pe(1, 1);
	be(0, 6) = Pe(2, 1); be(0, 9) = Pe(3, 1);

	// Second row
	be(1, 1) = Pe(0, 2); be(1, 4) = Pe(1, 2);
	be(1, 7) = Pe(2, 2); be(1, 10) = Pe(3, 2);

	// Third row
	be(2, 2) = Pe(0, 3); be(2, 5) = Pe(1, 3);
	be(2, 8) = Pe(2, 3); be(2, 11) = Pe(3, 3);

	// Fourth row
	be(3, 0) = 0.5 * Pe(0, 2); be(3, 1) = 0.5 * Pe(0, 1);
	be(3, 3) = 0.5 * Pe(1, 2); be(3, 4) = 0.5 * Pe(1, 1);
	be(3, 6) = 0.5 * Pe(2, 2); be(3, 7) = 0.5 * Pe(2, 1);
	be(3, 9) = 0.5 * Pe(3, 2); be(3, 10) = 0.5 * Pe(3, 1);

	// Fifth row
	be(4, 1) = 0.5 * Pe(0, 3); be(4, 2) = 0.5 * Pe(0, 2);
	be(4, 4) = 0.5 * Pe(1, 3); be(4, 5) = 0.5 * Pe(1, 2);
	be(4, 7) = 0.5 * Pe(2, 3); be(4, 8) = 0.5 * Pe(2, 2);
	be(4, 10) = 0.5 * Pe(3, 3); be(4, 11) = 0.5 * Pe(3, 2);

	// Sixth row
	be(5, 0) = 0.5 * Pe(0, 3); be(5, 2) = 0.5 * Pe(0, 1);
	be(5, 3) = 0.5 * Pe(1, 3); be(5, 5) = 0.5 * Pe(1, 1);
	be(5, 6) = 0.5 * Pe(2, 3); be(5, 8) = 0.5 * Pe(2, 1);
	be(5, 9) = 0.5 * Pe(3, 3); be(5, 11) = 0.5 * Pe(3, 1);

	return be;
}

Eigen::Matrix<double, 12, 12> FEA::BuildKe(glm::vec3 x1, glm::vec3 x2, glm::vec3 x3, glm::vec3 x4, double mu, double lambda)
{
	auto Pe = BuildBarycentricMatrix(x1, x2, x3, x4); // H matrix in PBR

	// volume matrix - +ve/-ve depending on winding order of nodes, that's why its important to get these correct to get +ve volume
	// TODO: assert volume positive
	auto vol = (1.0 / 6.0) / Pe.determinant();

	auto Be = BuildBe(Pe); // Displacement-deformation matrix (B matrix in PBR)

	Eigen::Matrix<double, 6, 6> E; // isotropic elasticity matrix (PBR calls this D matrix, is E matrix in GPP)
	E.setZero();

	auto mr = 2 * mu + lambda;

	E(0, 0) = mr; E(0, 1) = lambda; E(0, 2) = lambda;
	E(1, 0) = lambda; E(1, 1) = mr; E(1, 2) = lambda;
	E(2, 0) = lambda; E(2, 1) = lambda; E(2, 2) = mr;
	E(3, 3) = mu; E(4, 4) = mu; E(5, 5) = mu;

	auto Ke = vol * Be.transpose() * E * Be;

	return Ke;
}

void FEA::AssembleGlobalStiffnessMatrix(double mu, double lambda)
{
	auto elements = m_mesh->GetElements();
	auto nodes = m_mesh->GetNodes();

	auto result_matrix_size = 3 * nodes.size();

	Eigen::MatrixXd K(result_matrix_size, result_matrix_size);
	K.setZero();

	for (auto element : elements)
	{
		std::array<int, 4> m_indices;
		m_indices[0] = element.a; m_indices[1] = element.b; m_indices[2] = element.c; m_indices[3] = element.d;

		auto Ke = BuildKe(nodes[element.a], nodes[element.b], nodes[element.c], nodes[element.d], mu, lambda);

		for (auto source_row_index = 0; source_row_index < 4; ++source_row_index)
		{
			for (auto source_col_index = 0; source_col_index < 4; ++source_col_index)
			{
				auto dest_row_node_index = m_indices[source_row_index] * 3;
				auto dest_col_node_index = m_indices[source_col_index] * 3;

				for (auto vec3_row_index = 0; vec3_row_index < 3; ++vec3_row_index)
				{
					for (auto vec3_col_index = 0; vec3_col_index < 3; ++vec3_col_index)
					{
						K(dest_row_node_index + vec3_row_index, dest_col_node_index + vec3_col_index)
							+= Ke(source_row_index * 3 + vec3_row_index, source_col_index * 3 + vec3_col_index);
					}
				}
			}
		}
	}

	m_stiffness_matrix = K;
}

void FEA::CreateFloatingNodeStiffnessMatrix()
{
	// boundary conditions
	auto fixed_nodes = m_mesh->GetFixedNodes();

	auto nodes = m_mesh->GetNodes();
	auto nodes_count = nodes.size();
	m_original_node_indices_mapped_to_non_fixed.reserve(nodes_count);

	auto matrix_size = nodes_count * 3;
	auto resulting_size = matrix_size - (fixed_nodes.size() * 3);
	Eigen::MatrixXd resulting(resulting_size, resulting_size);
	resulting.setZero();

	auto non_fixed_node_num = 0;
	for (auto node_index = 0; node_index < nodes_count; node_index++)
	{
		if (std::find(begin(fixed_nodes), end(fixed_nodes), node_index) != end(fixed_nodes))
		{
			m_original_node_indices_mapped_to_non_fixed.push_back(-1);
			continue; 
		}

		m_original_node_indices_mapped_to_non_fixed.push_back(non_fixed_node_num);
		non_fixed_node_num++;
	}

	auto current_result_row = 0;
	for (auto row_index = 0; row_index < matrix_size; ++row_index)
	{
		auto row_node = static_cast<int>(row_index / 3);
		if (m_original_node_indices_mapped_to_non_fixed[row_node] == -1) { continue; }

		auto current_result_column = 0;
		for (auto column_index = 0; column_index < matrix_size; ++column_index)
		{
			auto column_node = static_cast<int>(column_index / 3);
			if (m_original_node_indices_mapped_to_non_fixed[column_node] == -1) { continue; }

			resulting(current_result_row, current_result_column) = m_stiffness_matrix(row_index, column_index);
			current_result_column++;

			if (current_result_column >= resulting_size) { break; } // copied all column data
		}

		current_result_row++;
		if (current_result_row >= resulting_size) { break; } // copied all row data
	}

	m_floating_node_stiffness_matrix = resulting;
}
