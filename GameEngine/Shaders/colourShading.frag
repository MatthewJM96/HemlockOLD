#version 130

in vec2 fragmentPosition;
in vec4 fragmentColour;

uniform float deltaTime;

out vec4 colour;

void main() {
    colour = vec4(  ( ( ( cos( fragmentPosition.x * 12.0f + ( deltaTime / 50.0f ) ) + 1 ) / 2 ) * fragmentColour.r ), 
					( ( ( sin( fragmentPosition.y * 16.0f + ( deltaTime / 150.0f ) ) + 1 ) / 2 ) * fragmentColour.g ), 
					( ( ( cos( fragmentPosition.x *  8.0f + ( deltaTime / 100.0f ) ) + 1 ) / 2 ) * fragmentColour.b ), 
					fragmentColour.a );
}