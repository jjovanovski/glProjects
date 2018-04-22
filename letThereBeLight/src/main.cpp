#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>

#include "shader.hpp"

// cube vertices and normal vectors
float cubeVerts[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

int main(int argc, char** args) {
	glfwInit();
		
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* window = glfwCreateWindow(600, 600, "Let There Be Light", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window.";
        return -1;
    }
	
	glfwMakeContextCurrent(window);
    gladLoadGL();

	// buffer object verts
	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// buffer light verts
	GLuint lightVao, lightVbo;
	glGenVertexArrays(1, &lightVao);
	glGenBuffers(1, &lightVbo);
	glBindVertexArray(lightVao);
	glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	Shader lightShader("lightvert.sh", "lightfrag.sh");
	Shader objectShader("vert.sh", "frag.sh");

	glm::mat4 lightModel = glm::mat4(1.0f);
	glm::mat4 objectModel = glm::mat4(1.0f);

	glm::mat4 view = glm::mat4(1.0f);
	glm::vec3 viewPos = glm::vec3(0.0f, 0.0f, -4.0f);
	view = glm::translate(view, viewPos);
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.01f, 100.0f);

	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 objectColor = glm::vec4(180/255.0f, 31/255.0f, 239/255.0f, 1.0f);
	objectShader.use();
	objectShader.setVec4("lightColor", lightColor);
	objectShader.setVec4("objectColor", objectColor);
	objectShader.setVec3("viewPos", -viewPos);

	float lightPosAngle = 0.0f;
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

		glm::vec3 lightPos = 5.0f * glm::vec3(cos(glm::radians(lightPosAngle)), 0.0f, sin(glm::radians(lightPosAngle)));
		lightModel = glm::mat4(1.0f);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f, 0.2f, 0.2f));
		lightModel = glm::translate(lightModel, lightPos);
		objectShader.setVec3("lightPos", lightPos);
		lightPosAngle += 1.0f;

		objectModel = glm::mat4(1.0f);
		objectModel = glm::rotate(objectModel, glm::radians(lightPosAngle/2), glm::vec3(1.0f, 1.0f, 1.0f));

        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		lightShader.use();
		lightShader.setMatrix("model", glm::value_ptr(lightModel));
		lightShader.setMatrix("view", glm::value_ptr(view));
		lightShader.setMatrix("projection", glm::value_ptr(projection));
		glBindVertexArray(lightVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		objectShader.use();
		objectShader.setMatrix("model", glm::value_ptr(objectModel));
		objectShader.setMatrix("view", glm::value_ptr(view));
		objectShader.setMatrix("projection", glm::value_ptr(projection));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
	
	glfwTerminate();	
	return 0;
}