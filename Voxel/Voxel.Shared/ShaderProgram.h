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

#include <string>

// TODO: this is all very hardcoded - need to see how to manage
//  dynamic shaders (provided and loaded by engine) and how that ties into
//  models also dynamically loaded. How to be flexible
//  e.g. if model does or not provide normals, or texture coordinates etc.
class ShaderProgram
{
public:
	ShaderProgram();
	~ShaderProgram();
public:
	// TODO: make private with accessors, or have a named-value lookup based on a map?
	GLuint m_program;

	GLint m_position_attrib_location;
	GLint m_normals_attrib_location;
	GLint m_color_attrib_location;

	GLint m_model_uniform_location;
	GLint m_view_uniform_location;
	GLint m_proj_uniform_location;
	GLint m_normals_uniform_location;

	GLint m_light_color_uniform_location;
	GLint m_light_ambient_intensity_uniform_location;
	GLint m_light_diffuse_intensity_uniform_location;
	GLint m_light_direction_uniform_location;

private:
	static const std::string FragmentShader;
	static const std::string VertexShader;
};

// Free functions
// TODO: put in namespace
GLuint CompileShader(GLenum type, const std::string &source);
GLuint CompileProgram(std::string vertex_shader, std::string fragment_shader);