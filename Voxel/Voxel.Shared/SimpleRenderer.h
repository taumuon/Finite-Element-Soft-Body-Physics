#pragma once

#include <string.h>

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

#include <memory>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class DrawableMesh;
class ShaderProgram;

struct DirectionalLight
{
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
	glm::vec3 Direction;
};

class SimpleRenderer
{
public:
    SimpleRenderer(std::shared_ptr<ShaderProgram> shader_program);
    ~SimpleRenderer();
    void Draw();
    void UpdateWindowSize(GLsizei width, GLsizei height);
	void AddMesh(std::shared_ptr<DrawableMesh> mesh);

private:
	GLsizei m_window_width;
	GLsizei m_window_height;

	std::vector<std::shared_ptr<DrawableMesh>> m_meshes;

	glm::mat4 m_projection_matrix;

	glm::vec4 m_light_direction_world_space;

	DirectionalLight m_light;

	std::shared_ptr<ShaderProgram> m_shader_program;
};
