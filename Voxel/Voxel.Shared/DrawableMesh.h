#pragma once

#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

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

struct Mesh;
class RenderMesh;

class DrawableMesh
{
private:
	std::shared_ptr<RenderMesh> m_mesh;
	glm::mat4 m_model_matrix;
public:
	DrawableMesh(std::shared_ptr<RenderMesh> mesh);
	~DrawableMesh();

	void Draw(glm::mat4 view, glm::mat4 proj, GLuint position_attrib_location, GLuint color_attrib_location, GLuint normals_attrib_location);

	void UpdateMesh(Mesh mesh);

	glm::mat4 GetModelMatrix() { return m_model_matrix; }
	void SetModelMatrix(glm::mat4 model_matrix) { m_model_matrix = model_matrix; }
};

