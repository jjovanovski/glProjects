#version 330 core

in vec3 fragmentPos;
in vec3 fragmentNormal;

out vec4 colorOut;

uniform vec4 objectColor;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	float ambientScalar = 0.1f;
	float specularStrength = 0.5f;

	// calculate diffuse
	vec3 lightDir = normalize(lightPos - fragmentPos);
	float diff = max(dot(fragmentNormal, lightDir), 0.0);

	// calculate specular
	vec3 viewDir = normalize(viewPos - fragmentPos);
	vec3 reflectDir = reflect(-lightDir, fragmentNormal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
	vec4 specular = specularStrength * spec * lightColor;

	vec4 diffuse = lightColor*diff;
	vec4 ambient = objectColor * ambientScalar;
	colorOut = (ambient + diffuse + specular) * objectColor;
}