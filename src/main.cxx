#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <array>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Debug.h"
#include "Shader.h"
#include "Mesh.h"
#include "TexturedMesh.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

namespace fs = std::filesystem;

constexpr int SCR_WIDTH = 640;
constexpr int SCR_HEIGHT = 480;

int main()
{
	if (!glfwInit())
	{
		std::cerr << "GLFW failed to init" << '\n';
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // __APPLE__
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Window failed to create" << '\n';
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cerr << "Failed to initilize GLAD" << '\n';
		return -1;
	}

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		// initialize debug output
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	glm::mat4 projection(glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), static_cast<float>(SCR_HEIGHT), 0.0f));

	Shader color(std::ifstream("ColorAndText.vert"), std::ifstream("ColorAndText.frag"));
	int colorLoc = glGetUniformLocation(color, "color");
	glUseProgram(color);
	glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));


	std::vector<float> vertices{
		// positions	// tex coords
		 0.5f,  0.5f,	1.0f, 1.0f,	// right top  
		 0.5f, -0.5f,	1.0f, 0.0f,	// right bottom
		-0.5f, -0.5f,	0.0f, 0.0f,	// left bottom
		-0.5f,  0.5f,	0.0f, 1.0f,	// left top 
	};

	std::vector<GLuint> indices{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	TexturedMesh testMesh;
	try
	{
		testMesh = TexturedMesh(vertices, indices, "container.jpg");
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << e.what() << '\n';
		return 1;
	}

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(color);
		testMesh.draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}