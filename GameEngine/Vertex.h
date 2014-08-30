#pragma once

#include <GL/glew.h>

struct Position {
	float x;
	float y;
};

struct Colour {
	GLubyte r;
	GLubyte g;
	GLubyte b;
	GLubyte a;
};

struct Vertex {
	Position position;
	Colour colour;
	//Vertex must be a mutliple of 4 bytes for alignment purposes, so use padding variables when necessary!
};