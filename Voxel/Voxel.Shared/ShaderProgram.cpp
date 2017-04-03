#include "pch.h"
#include "ShaderProgram.h"

#include <vector>
#include <iostream>

ShaderProgram::ShaderProgram()
{
	// TODO: instead have a map of the attrib and uniform locations ?
	m_program = CompileProgram(VertexShader, FragmentShader);
	m_position_attrib_location = glGetAttribLocation(m_program, "aPosition");
	m_color_attrib_location = glGetAttribLocation(m_program, "aColor");
	m_normals_attrib_location = glGetAttribLocation(m_program, "aNormal");
	m_model_uniform_location = glGetUniformLocation(m_program, "uModelMatrix");
	m_view_uniform_location = glGetUniformLocation(m_program, "uViewMatrix");
	m_proj_uniform_location = glGetUniformLocation(m_program, "uProjMatrix");
	m_normals_uniform_location = glGetUniformLocation(m_program, "uNormalsMatrix");

	m_light_color_uniform_location = glGetUniformLocation(m_program, "light.Color");
	m_light_ambient_intensity_uniform_location = glGetUniformLocation(m_program, "light.AmbientIntensity");
	m_light_diffuse_intensity_uniform_location = glGetUniformLocation(m_program, "light.DiffuseIntensity");
	m_light_direction_uniform_location = glGetUniformLocation(m_program, "light.Direction");
}


ShaderProgram::~ShaderProgram()
{
	if (m_program != 0)
	{
		glDeleteProgram(m_program);
		m_program = 0;
	}
}

GLuint CompileShader(GLenum type, const std::string &source)
{
	GLuint shader = glCreateShader(type);

	const char *sourceArray[1] = { source.c_str() };
	glShaderSource(shader, 1, sourceArray, NULL);
	glCompileShader(shader);

	GLint compile_result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	if (compile_result == 0)
	{
		GLint info_log_length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);

		std::vector<GLchar> info_log(info_log_length);
		glGetShaderInfoLog(shader, (GLsizei)info_log.size(), NULL, info_log.data());

		std::string error_message = std::string("Shader compilation failed: ");
		error_message += std::string(info_log.begin(), info_log.end());

		// throw std::runtime_error(errorMessage.c_str()); // not available in android
		throw error_message;
	}

	return shader;
}

GLuint CompileProgram(std::string vertex_shader, std::string fragment_shader)
{
	GLuint program = glCreateProgram();

	if (program == 0)
	{
		// throw std::runtime_error("Program creation failed"); // not available in android
		throw "Program creation failed";
	}

	GLuint vs = CompileShader(GL_VERTEX_SHADER, vertex_shader);
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragment_shader);

	if (vs == 0 || fs == 0)
	{
		glDeleteShader(fs);
		glDeleteShader(vs);
		glDeleteProgram(program);
		return 0;
	}

	glAttachShader(program, vs);
	glDeleteShader(vs);

	glAttachShader(program, fs);
	glDeleteShader(fs);

	glLinkProgram(program);

	GLint link_status;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);

	if (link_status == 0)
	{
		GLint info_log_length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);

		std::vector<GLchar> info_log(info_log_length);
		glGetProgramInfoLog(program, (GLsizei)info_log.size(), NULL, info_log.data());

		std::string error_message = std::string("Program link failed: ");
		error_message += std::string(info_log.begin(), info_log.end());

		// throw std::runtime_error(errorMessage.c_str()); // not available in android
		throw error_message;
	}

	return program;
}

const std::string ShaderProgram::VertexShader = R"(
		#version 300 es

        uniform mat4 uModelMatrix;
		uniform mat4 uViewMatrix;
        uniform mat4 uProjMatrix;
		uniform mat4 uNormalsMatrix;
        in vec4 aPosition;
        in vec4 aColor;
		in vec4 aNormal;
		out vec3 vNormal;
        out vec4 vColor;

        void main()
        {
            gl_Position = uProjMatrix * uViewMatrix * uModelMatrix * aPosition;
			vNormal = (uNormalsMatrix * aNormal).xyz;
            vColor = aColor;
        }
    )";

const std::string ShaderProgram::FragmentShader = R"(
		#version 300 es

        precision highp float;

		struct DirectionalLight
		{
			vec3 Color;
			float AmbientIntensity;
			float DiffuseIntensity;
			vec3 Direction;
		};

		uniform DirectionalLight light;

        in vec4 vColor;
		in vec3 vNormal;

		out vec4 fragColor;

        void main()
        {
			//calculate the cosine of the angle of incidence
			// TODO: should change the view, to ensure that light direction is in the right space.
			float diffuseFactor = dot(normalize(vNormal), normalize(light.Direction));
			diffuseFactor = clamp(diffuseFactor, 0.0, 1.0);

			float brightness = clamp(light.AmbientIntensity + (light.DiffuseIntensity * diffuseFactor), 0.0, 1.0);

			// TODO: delete me, this is just to remove the diffuse lighting without anything being optimised out
			// brightness = clamp(1.0 + brightness, 0.0, 1.0);

            fragColor = vec4(vColor.rgb * brightness, 1.0);
        }
    )";