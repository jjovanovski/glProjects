//
//  shader.hpp
//  Glitter
//
//  Created by Jovan Jovanovski on 2/27/18.
//

#ifndef shader_hpp
#define shader_hpp

#include <glad/glad.h>

#include <string>
#include <stdio.h>
#include <vector>
#include <unordered_map>

#endif /* shader_hpp */


class Shader
{
public:
    Shader(const GLchar* vertexShaderFile, const GLchar* fragmentShaderFile);
    ~Shader();
    
    void use();
    void deleteProgram();
    
	// uniform setters
	void setVec4(const std::string name, std::vector<GLfloat> vec);
	void setMatrix(const std::string name, GLfloat* dataPointer);
    
private:
    GLuint shaderProgramId;
	std::unordered_map<std::string, GLint> uniforms;
    
    std::string readFile(const GLchar* filePath);
};
