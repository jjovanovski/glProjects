#version 330 core

layout (location = 0) in vec3 posIn;
layout (location = 1) in vec3 normalIn;

uniform mat4 model, view, projection;

out vec3 fragmentPos;
out vec3 fragmentNormal;

void main() {
	gl_Position = projection * view * model * vec4(posIn, 1.0);
	fragmentPos = vec3(model * vec4(posIn, 1.0));
	fragmentNormal = mat3(transpose(inverse(model))) * normalIn;
}