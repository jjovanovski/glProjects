#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cmath>

#include "shader.hpp"

struct vertex {
	float x, y, z;
};

int main(int argc, char** args) {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "glBasic", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create window.";
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();

	Shader shader("vert.sh", "frag.sh");

	std::vector<float> verts;

	float PI = 3.14f;
	int numberOfLines = 30;
	float angleStep = (2 * PI) / numberOfLines;
	float angle = 0.0f;

	for (int i = 0; i < numberOfLines; i++) {
		float x = cos(angle);
		float y = sin(angle);
		float z = 0.0f;

		verts.push_back(x);
		verts.push_back(y);
		verts.push_back(z);

		// increase the angle
		angle = angle + angleStep;
	}
	verts.push_back(0.0f);
	verts.push_back(0.0f);
	verts.push_back(0.0f);

	std::vector<int> indices;

	for (int i = 0; i < numberOfLines; i++) {
		indices.push_back(numberOfLines);
		indices.push_back(i);
		indices.push_back((i + 1) % numberOfLines);
	}

	GLuint vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex)*verts.size(), &verts[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*indices.size(), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glBindVertexArray(vao);
		shader.use();
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &ebo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	glfwTerminate();
	return 0;
}