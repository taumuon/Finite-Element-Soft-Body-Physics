//
// This file is used by the template to render a basic scene using GL.
//
#include "pch.h"

#include "SimpleRenderer.h"
#include "ShaderProgram.h"

#include "DrawableMesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/matrix_inverse.hpp>

// These are used by the shader compilation methods.
#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

SimpleRenderer::SimpleRenderer(std::shared_ptr<ShaderProgram> shader_program)
	: m_window_width(0),
      m_window_height(0),
	  m_light_direction_world_space(glm::vec4 { 1.0f, 1.0f, 0.0f, 0.0f }),
	  m_shader_program(shader_program)
{
	m_light.AmbientIntensity = 0.1f;
	m_light.DiffuseIntensity = 1.0f;
	m_light.Color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_light.Direction = glm::vec3(0.0f, 0.0f, 1.0f);

	// explanation of uNormalMatrix and phong lighting
	// http://www.tomdalling.com/blog/modern-opengl/06-diffuse-point-lighting/
		/*dot(v1, v2) == length(v1)*length(v2)*cos(angle)
		dot(v1, v2) / (length(v1)*length(v2)) == cos(angle)
		acos(dot(v1, v2) / (length(v1)*length(v2))) == angle*/
}

SimpleRenderer::~SimpleRenderer()
{
}

void SimpleRenderer::Draw()
{
    glEnable(GL_DEPTH_TEST);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_shader_program->m_program);
	
	// side-on view
	glm::mat4 view_matrix = glm::lookAt(glm::vec3(0.0f, 2.0f, -10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// top-down view
	// glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 8.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(m_shader_program->m_view_uniform_location, 1, GL_FALSE, &view_matrix[0][0]);

    glUniformMatrix4fv(m_shader_program->m_proj_uniform_location, 1, GL_FALSE, &m_projection_matrix[0][0]);

	// TODO: as different models could have different shaders, shouldn't each model
	//  populate its shader with its lighting - i.e. pull out of here!

	glm::vec3 light_color{ 1.0f, 1.0f, 1.0f };
	glUniform3fv(m_shader_program->m_light_color_uniform_location, 1, &light_color[0]);

	glUniform1f(m_shader_program->m_light_ambient_intensity_uniform_location, 0.2f);

	glUniform1f(m_shader_program->m_light_diffuse_intensity_uniform_location, 1.0f);

	for (auto mesh : m_meshes)
	{
		glm::mat4 model_matrix = mesh->GetModelMatrix();
		glUniformMatrix4fv(m_shader_program->m_model_uniform_location, 1, GL_FALSE, &model_matrix[0][0]);

		// TODO: pass mvp matrix into shader
		glm::mat4 model_view_matrix = view_matrix * model_matrix;

		auto normals_matrix = glm::inverseTranspose(model_view_matrix);

		glUniformMatrix4fv(m_shader_program->m_normals_uniform_location, 1, GL_FALSE, &normals_matrix[0][0]);

		glm::vec4 light_direction_model_view_space = view_matrix * m_light_direction_world_space;
		glUniform3fv(m_shader_program->m_light_direction_uniform_location, 1, &light_direction_model_view_space[0]);

		mesh->Draw(view_matrix, m_projection_matrix, m_shader_program->m_position_attrib_location, m_shader_program->m_color_attrib_location, m_shader_program->m_normals_attrib_location);
	}
}

void SimpleRenderer::UpdateWindowSize(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
    m_window_width = width;
    m_window_height = height;

	// Projection matrix : 45degree Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	m_projection_matrix = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

void SimpleRenderer::AddMesh(std::shared_ptr<DrawableMesh> mesh)
{
	m_meshes.push_back(mesh);
}
