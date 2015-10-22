#version 130

in vec2 fragmentPosition;
in vec4 fragmentColour;
in vec2 fragmentUV;

out vec4 colour;

uniform sampler2D tex;

void main() {
	vec4 textureColour = texture(tex, fragmentUV);

	colour = textureColour * fragmentColour;
	
	//colour += vec4 (
	//							1.0 * (cos(time) + fragmentPosition.x) * 0.5,
	//							1.0 * (cos(time + fragmentPosition.y + 1.0) + 1.0) * 0.5,
	//							1.0 * (sin(time + 1.0) + 1.0) * 0.5,
	//							1.0
	//						);
}