#pragma once

#include <GL/glew.h>

namespace Xylem
{
    struct Position {
        float x;
        float y;
    };

    struct ColourRGBA8 {
        ColourRGBA8() : r(0), g(0), b(0), a(0) {}
        ColourRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A) :
            r(R), g(G), b(B), a(A) {}
        GLubyte r;
        GLubyte g;
        GLubyte b;
        GLubyte a;
    };

    struct UV {
        float u;
        float v;
    };

    struct Vertex {
        Position position;
        ColourRGBA8 colour;
        UV uv;

        void setPosition(float x, float y) {
            position.x = x;
            position.y = y;
        }
        void setColour(GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
            colour.r = r;
            colour.g = g;
            colour.b = b;
            colour.a = a;
        }
        void setUV(float u, float v) {
            uv.u = u;
            uv.v = v;
        }
    };
}