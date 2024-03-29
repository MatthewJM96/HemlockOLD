#version 130

in vec2 vertexPosition;
in vec4 vertexColour;

uniform float deltaTime;

out vec2 fragmentPosition;
out vec4 fragmentColour;

void main() {
    gl_Position.xy = vertexPosition;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

	fragmentPosition = vertexPosition;
	fragmentColour = vertexColour;
}