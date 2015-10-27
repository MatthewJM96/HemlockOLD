#pragma once

#include "Vertex.h"

#include <GL\glew.h>
#include <GLM\glm.hpp>

#include <vector>

namespace Xylem
{
    enum class GlyphSortType
    {
        NONE,
        FRONT_TO_BACK,
        BACK_TO_FRONT,
        TEXTURE
    };

    struct Glyph
    {
        GLuint texture;
        float depth;

        Vertex topLeft;
        Vertex bottomLeft;
        Vertex topRight;
        Vertex bottomRight;
    };

    class RenderBatch
    {
    public:
        RenderBatch(GLuint offset_, GLuint vertexCount_, GLuint texture_)
            : offset(offset_),
            vertexCount(vertexCount_),
            texture(texture_)
        {}
        GLuint offset;
        GLuint vertexCount;
        GLuint texture;
    };

    class SpriteBatch
    {
    public:
        SpriteBatch();
        ~SpriteBatch();

        void init();

        void begin(GlyphSortType sortType = GlyphSortType::TEXTURE);
        void end();

        void draw(const glm::vec4& destinationRectangle, const glm::vec4& uvRectangle, GLuint texture, float depth, const Colour& colour);

        void renderBatch();
    private:
        void createRenderBatches();
        void createVertexArray();
        void sortGlyphs();

        static bool compareFrontToBack(Glyph* a, Glyph* b);
        static bool compareBackToFront(Glyph* a, Glyph* b);
        static bool compareTexture(Glyph* a, Glyph* b);

        GLuint _vbo;
        GLuint _vao;

        GlyphSortType _sortType;

        std::vector<Glyph*> _glyphs;
        std::vector<RenderBatch> _renderBatches;
    };
}