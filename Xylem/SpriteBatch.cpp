#include "SpriteBatch.h"

#include <algorithm>

namespace Xylem
{
    SpriteBatch::SpriteBatch()
        : _vbo(0),
        _vao(0)
    {
    }

    SpriteBatch::~SpriteBatch()
    {
    }

    /// Initialises the sprite batch.
    void SpriteBatch::init()
    {
        createVertexArray();
    }

    /// Prepares the sprite batch for drawing.
    void SpriteBatch::begin(GlyphSortType sortType/*= GlyphSortType::TEXTURE*/)
    {
        _sortType = sortType;
        _renderBatches.clear();
        for (size_t i = 0; i < _glyphs.size(); ++i) {
            delete _glyphs[i];
        }
        _glyphs.clear();
    }

    /// Ends the sprite batch's draw mode.
    void SpriteBatch::end()
    {
        sortGlyphs();
        createRenderBatches();
    }

    /// Prepares a sprite by drawing in memory.
    void SpriteBatch::draw(const glm::vec4 & destinationRectangle, const glm::vec4 & uvRectangle, GLuint texture, float depth, const ColourRGBA8 & colour)
    {
        Glyph* newGlyph = new Glyph();
        newGlyph->texture = texture;
        newGlyph->depth = depth;

        newGlyph->topLeft.colour = colour;
        newGlyph->topLeft.setPosition(destinationRectangle.x, destinationRectangle.y + destinationRectangle.w);
        newGlyph->topLeft.setUV(uvRectangle.x, uvRectangle.y + uvRectangle.w);

        newGlyph->bottomLeft.colour = colour;
        newGlyph->bottomLeft.setPosition(destinationRectangle.x, destinationRectangle.y);
        newGlyph->bottomLeft.setUV(uvRectangle.x, uvRectangle.y);

        newGlyph->topRight.colour = colour;
        newGlyph->topRight.setPosition(destinationRectangle.x + destinationRectangle.z, destinationRectangle.y + destinationRectangle.w);
        newGlyph->topRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y + uvRectangle.w);

        newGlyph->bottomRight.colour = colour;
        newGlyph->bottomRight.setPosition(destinationRectangle.x + destinationRectangle.z, destinationRectangle.y);
        newGlyph->bottomRight.setUV(uvRectangle.x + uvRectangle.z, uvRectangle.y);

        _glyphs.push_back(newGlyph);
    }

    /// Renders all the drawn sprites.
    void SpriteBatch::renderBatch()
    {
        glBindVertexArray(_vao);

        for (size_t i = 0; i < _renderBatches.size(); ++i) {
            glBindTexture(GL_TEXTURE_2D, _renderBatches[i].texture);

            glDrawArrays(GL_TRIANGLES, _renderBatches[i].offset, _renderBatches[i].vertexCount);
        }

        glBindVertexArray(0);
    }

    /// Creates the render batches.
    void SpriteBatch::createRenderBatches()
    {
        std::vector<Vertex> vertices;
        vertices.resize(_glyphs.size() * 6);

        if (_glyphs.empty()) {
            return;
        }

        int offset = 0;
        int currentVertex = 0;

        _renderBatches.emplace_back(offset, 6, _glyphs[0]->texture);
        vertices[currentVertex++] = _glyphs[0]->topLeft;
        vertices[currentVertex++] = _glyphs[0]->bottomLeft;
        vertices[currentVertex++] = _glyphs[0]->bottomRight;
        vertices[currentVertex++] = _glyphs[0]->bottomRight;
        vertices[currentVertex++] = _glyphs[0]->topRight;
        vertices[currentVertex++] = _glyphs[0]->topLeft;
        offset += 6;

        for (size_t currentGlyph = 1; currentGlyph < _glyphs.size(); ++currentGlyph) {
            if (_glyphs[currentGlyph]->texture != _glyphs[currentGlyph - 1]->texture) {
                _renderBatches.emplace_back(offset, 6, _glyphs[currentGlyph]->texture);
            } else {
                _renderBatches.back().vertexCount += 6;
            }
            vertices[currentVertex++] = _glyphs[currentGlyph]->topLeft;
            vertices[currentVertex++] = _glyphs[currentGlyph]->bottomLeft;
            vertices[currentVertex++] = _glyphs[currentGlyph]->bottomRight;
            vertices[currentVertex++] = _glyphs[currentGlyph]->bottomRight;
            vertices[currentVertex++] = _glyphs[currentGlyph]->topRight;
            vertices[currentVertex++] = _glyphs[currentGlyph]->topLeft;
            offset += 6;
        }

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        // Orphans the buffer.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    /// Creates a vertex array.
    void SpriteBatch::createVertexArray()
    {
        if (_vao == 0) {
            glGenVertexArrays(1, &_vao);
        }
        glBindVertexArray(_vao);

        if (_vbo == 0) {
            glGenBuffers(1, &_vbo);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
        glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, colour));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

        glBindVertexArray(0);
    }

    /// Sorts the glyphs in method specified in the begin function.
    void SpriteBatch::sortGlyphs()
    {
        switch (_sortType) {
        case GlyphSortType::BACK_TO_FRONT:
            std::stable_sort(_glyphs.begin(), _glyphs.end(), compareBackToFront);
            break;
        case GlyphSortType::FRONT_TO_BACK:
            std::stable_sort(_glyphs.begin(), _glyphs.end(), compareFrontToBack);
            break;
        case GlyphSortType::TEXTURE:
            std::stable_sort(_glyphs.begin(), _glyphs.end(), compareTexture);
            break;
        }
    }

    /// Compares glyphs front to back.
    bool SpriteBatch::compareFrontToBack(Glyph * a, Glyph * b)
    {
        return (a->depth < b->depth);
    }

    /// Compares glyphs back to front.
    bool SpriteBatch::compareBackToFront(Glyph * a, Glyph * b)
    {
        return (a->depth > b->depth);
    }

    /// Compares glyphs by texture type.
    bool SpriteBatch::compareTexture(Glyph * a, Glyph * b)
    {
        return (a->texture < b->texture);
    }
}