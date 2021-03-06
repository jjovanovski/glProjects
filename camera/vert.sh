#version 330 core

layout (location = 0) in vec3 posIn;
layout (location = 1) in vec2 texCoordsIn;

out vec2 texCoords;

uniform mat4 model, view, projection;

void main() {
	gl_Position = projection * view * model * vec4(posIn, 1.0);
	texCoords = texCoordsIn;
}