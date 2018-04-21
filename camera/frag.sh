#version 330 core

in vec2 texCoords;

out vec4 colorOut;

uniform sampler2D texUni;

void main() {
	colorOut = texture(texUni, texCoords);
}