#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>

Sprite::Sprite()
	: _isInit(false), _vboID(0)
{
}

Sprite::~Sprite()
{
	if (_vboID != 0) {
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::init(Vec2D <float> position, Vec2D <float> dimensions)
{
	_pos = position;
	_dims = dimensions;

	if (!_vboID) {
		glGenBuffers(1, &_vboID);
	}

	Vertex vertexData[6];

	//First Triangle
	vertexData[0].position.x = _pos.x + _dims.x;
	vertexData[0].position.y = _pos.y + _dims.y;

	vertexData[1].position.x = _pos.x;
	vertexData[1].position.y = _pos.y + _dims.y;

	vertexData[2].position.x = _pos.x;
	vertexData[2].position.y = _pos.y;

	//Second Triangle
	vertexData[3].position.x = _pos.x;
	vertexData[3].position.y = _pos.y;

	vertexData[4].position.x = _pos.x + _dims.x;
	vertexData[4].position.y = _pos.y;

	vertexData[5].position.x = _pos.x + _dims.x;
	vertexData[5].position.y = _pos.y + _dims.y;

	for (size_t i = 0; i < 6; ++i) {
		vertexData[i].colour.r = 255;
		vertexData[i].colour.g = 0;
		vertexData[i].colour.b = 255;
		vertexData[i].colour.a = 255;
	}

	//Mix up the colours a bit
	vertexData[1].colour.r = 0;

	vertexData[4].colour.r = 0;
	vertexData[4].colour.g = 255;
	vertexData[4].colour.b = 0;

	//Bind buffer to _vboID
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	//Unbind buffer from _vboID
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	_isInit = true;
}

int Sprite::draw()
{
	if (!_isInit) {
		return 0;
	}

	//Bind buffer to _vboID
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);

	glEnableVertexAttribArray(0);

	//Position attribute pointer
	//0 - position is index 0
	//2 - two bits of data per vertex (i.e. in 2D)
	//GL_FLOAT - using floats in vertex data
	//GL_FALSE - doesn't need normalising
	//sizeof(Vertex) - size of Vertex as other data is in Vertex than just position
	//(void*) offsetof(Vertex, position) - offset into stride (dynamically calculated rather than statically entered as 0
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, position));

	//Colour attribute pointer
	//1 - colour is index 1
	//4 - four bytes of data per vertex (i.e. rgba)
	//GL_UNSIGNED_BYTE - using unsigned bytes in vertex data
	//GL_TRUE - normalise to between a value of 0 and 1 (i.e. convert rgba 0-255 scale to 0.0-1.0 scale)
	//sizeof(Vertex) - size of Vertex as other data is in Vertex than just position
	//0 - offset into stride
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));

	//GL_TRIANGLES - draw in triangles
	//0 - start at the start (no shit, eh?)
	//6 - 6 vertices to draw (info for each is passed in with glVertexAttribPointer)
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(0);

	//Unbind buffer from _vboID
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return 1;
}