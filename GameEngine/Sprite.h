#pragma once
#include "Vec2D.h"
#include <GL/glew.h>
class Sprite
{
public:
	Sprite();
	~Sprite();

	void init(Vec2D <float> position, Vec2D <float> dimensions);
	int draw();
private:
	bool _isInit;
	Vec2D <float> _pos;
	Vec2D <float> _dims;
	GLuint _vboID;
};

