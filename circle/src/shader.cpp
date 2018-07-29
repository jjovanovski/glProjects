//
//  shader.cpp
//  Glitter
//
//  Created by Jovan Jovanovski on 2/27/18.
//

#include "shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

/*
 Shader program process:
 1. Read vertex, fragment and geometry (optional) shaders source
 2. Compile the source
 3. Link the compiled shaders to shader program
 */
Shader::Shader(const GLchar* vertexShaderFile, const GLchar* fragmentShaderFile) {
    // TODO: check if files exsit and are readable
    
    std::string vertexSourceString = readFile(vertexShaderFile);
    std::string fragmentSourceString = readFile(fragmentShaderFile);
    const GLchar* vertexSource = vertexSourceString.c_str();
    const GLchar* fragmentSource = fragmentSourceString.c_str();
    
    int success;
    char infoLog[512];
    
    // compile vertex shader
    GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Failed to compile vertex shader: " << infoLog << std::endl;
        
        glDeleteShader(vertexShader);
        return;
    }
    
    // compile fragment shader
    GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Failed to compile fragment shader: " << infoLog << std::endl;
        
        glDeleteShader(fragmentShader);
        return;
    }
    
    // link both to shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << infoLog << std::endl;
    }
    
    // detach and delete shaders (shader program remains compiled&linked in memory)
    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    this->shaderProgramId = shaderProgram;
}


Shader::~Shader() {
    deleteProgram();
}

void Shader::use() {
    glUseProgram(shaderProgramId);
}

void Shader::deleteProgram() {
    glDeleteProgram(shaderProgramId);
}

std::string Shader::readFile(const GLchar * filePath) {
    std::ifstream file;
    file.open(filePath);
    
    if (!file.is_open()) {
        file.close();
        return "";
    }
    
    std::stringstream str;
    str << file.rdbuf();
    
    file.close();
    
    return str.str();
}

void Shader::setMatrix(const std::string name, GLfloat* dataPointer) {
	GLint uniformLocation = -1;
	if (uniforms.find(name) == uniforms.end()) {
		uniformLocation = glGetUniformLocation(shaderProgramId, name.c_str());
	} else {
		uniformLocation = uniforms[name];
	}

	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, dataPointer);
}

void Shader::setVec4(const std::string name, std::vector<GLfloat> vec) {
	GLint uniformLocation = -1;
	if (uniforms.find(name) == uniforms.end()) {
		uniformLocation = glGetUniformLocation(shaderProgramId, name.c_str());
	} else {
		uniformLocation = uniforms[name];
	}

	glUniform4f(uniformLocation, vec[0], vec[1], vec[2], vec[3]);
}