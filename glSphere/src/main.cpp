#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <vector>
#include <math.h>

#include "shader.hpp"

#define PI 3.141592f
#define DEG PI/180.0f

struct vertex {
	float x, y, z;
};

void genUVSphere(std::vector<vertex>& verts, std::vector<GLuint>& indices, int rings, int sectors) {
	float d_theta = 180.0f / rings;
	float d_phi = 360.0f / sectors;
	float theta = -90.0f;
	float phi = 0.0f;

	float x, y, z;
	for (int i = 0; i <= rings; i++) {
		phi = 0.0f;
		for (int j = 0; j < sectors; j++) {
			float x = cos(theta*DEG)*cos(phi*DEG);
			float y = cos(theta*DEG)*sin(phi*DEG);
			float z = sin(theta*DEG);

			verts.push_back({ x, z, y });

			if (j < sectors) {
				indices.push_back(i*(sectors) + j);
				indices.push_back(i*(sectors) + (j + 1)%sectors);
				indices.push_back((i + 1)*(sectors) +(j + 1) % sectors);

				indices.push_back(i*(sectors) + j);
				indices.push_back((i + 1)*(sectors) +(j + 1) % sectors);
				indices.push_back((i + 1)*(sectors) + j);
			}

			phi += d_phi;
		}
		theta += d_theta;
	}
}

int main(int argc, char** args) {
	glfwInit();
		
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* window = glfwCreateWindow(600, 600, "glSphere", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window.";
        return -1;
    }
	
	glfwMakeContextCurrent(window);
    gladLoadGL();

	Shader shader("vert.sh", "frag.sh");

	std::vector<vertex> verts;
	std::vector<GLuint> indices;
	genUVSphere(verts, indices, 30, 30);

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

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

		model = glm::rotate(model, glm::radians(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();
		shader.setMatrix("model", glm::value_ptr(model));
		shader.setMatrix("view", glm::value_ptr(view));
		shader.setMatrix("projection", glm::value_ptr(projection));

		glBindVertexArray(vao);
		shader.setVec4("myColor", { 0.0f, 0.0f, 0.0f, 1.0f });
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

		shader.setVec4("myColor", { 0.0f, 1.0f, 0.0f, 1.0f });
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	
	glfwTerminate();	
	return 0;
}