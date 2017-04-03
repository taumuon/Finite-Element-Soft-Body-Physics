#pragma once

#ifdef __APPLE__
#include <unistd.h>
#include <sys/resource.h>

#include <OpenGLES/ES2/gl.h>
#else // __ANDROID__ or _WIN32
#ifdef __GLEW__
// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#else
#include <GLES2/gl2.h>
#endif
#endif

#include "Mesh.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include <vector>
#include <memory>

class ShaderProgram;

class RenderMesh
{
public:
	RenderMesh(Mesh mesh,
		       std::shared_ptr<ShaderProgram> shader_program,
			   bool isStatic);
	~RenderMesh();

	void Draw(glm::mat4 model,
			  GLuint position_attrib_location,
			  GLuint color_attrib_location,
			  GLuint normals_attrib_location);

	void UpdateMesh(Mesh mesh, bool is_static = false);

private:
	GLuint m_vertex_position_buffer;
	GLuint m_vertex_color_buffer;
	GLuint m_vertex_normals_buffer;
	GLuint m_index_buffer;
	GLsizei m_index_count;

	std::shared_ptr<ShaderProgram> m_shader_program;
};

