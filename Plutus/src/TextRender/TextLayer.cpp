#include <glm/gtc/matrix_transform.hpp>
#include "TextLayer.h"
#include <Graphics/GLheaders.h>
// FreeType
#include <ft2build.h>
#include FT_FREETYPE_H

#if defined(__EMSCRIPTEN__) || defined(__ANDROID__)
#define VERTEX_HEADER "#version 300 es \nprecision mediump float;\n"
#else
#define VERTEX_HEADER "#version 330 core\n"
#endif

inline const std::string vertTextLayer = std::string(VERTEX_HEADER) + R"END(layout (location = 0) in vec4 vertex;
out vec2 TexCoords;

uniform mat4 projMatrix;

void main()
{
    gl_Position = projMatrix * vec4(vertex.xy, 0.0, 1.0);
    TexCoords = vertex.zw;
})END";

inline const std::string fragTextLayer = std::string(VERTEX_HEADER) + R"END(in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{    
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
    color = textColor * sampled;
})END";

namespace Plutus
{

    TextLayer::~TextLayer()
    {
        glDeleteBuffers(1, &pVBO);
        glDeleteVertexArrays(1, &pVAO);
        pShader.dispose();
    }

    bool TextLayer::Init(float width, float height, std::string fontPath, uint32_t fontSize)
    {
        pWidth = width;
        pHeight = height;
        if (!pShader.CreateProgWithShader(vertTextLayer, fragTextLayer))
        {
            std::cout << "Error Creating program \n";
            return false;
        }

        // enableBlendMode();

        pShader.enable();

        initFreetypeFont(fontPath, fontSize);
        pShader.setUniformMat4("projMatrix", glm::ortho(0.0f, pWidth, 0.0f, pHeight));
        pShader.disable();
        return true;
    }

    void TextLayer::initFreetypeFont(std::string fontPath, uint32_t fontSize)
    {
        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft))
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

        // Load font as face
        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face))
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

        // Set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, fontSize);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Load first 128 characters of ASCII set
        for (GLubyte c = 0; c < 128; c++)
        {
            // Load character glyph
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // Generate texture
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_R8,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer);
            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // Now store character for later use
            pCharset[c] = TextCharacter(
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        // Destroy FreeType once we're finished
        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        // Configure VAO/VBO for texture quads
        glGenVertexArrays(1, &pVAO);
        glGenBuffers(1, &pVBO);

        glBindVertexArray(pVAO);
        glBindBuffer(GL_ARRAY_BUFFER, pVBO);

        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
    /*
        Draw the text on on the screen
        @param text text to draw
        @param x,y position on the screen
        @param scale font size
        @param color color of the text
    */
    void TextLayer::drawString(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec4 color)
    {
        // Activate corresponding render state
        pShader.enable();
        pShader.setUniform4f("textColor", color);
        glActiveTexture(GL_TEXTURE0 + mActiveTexture);
        //pIBO->bind();
        glBindVertexArray(pVAO);
        GLfloat startX = x;
        // Iterate through all characters
        for (char& c : text)
        {
            if (c == '\n')
            {
                x = startX;
                y += 28;
                continue;
            }
            TextCharacter ch = pCharset[c];

            GLfloat xpos = x + ch.Bearing.x * scale;
            GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

            GLfloat w = ch.Size.x * scale;
            GLfloat h = ch.Size.y * scale;
            // Update VBO for each character
            GLfloat vertices[6][4] = {
                {xpos, ypos + h, 0.0, 0.0},
                {xpos, ypos, 0.0, 1.0},
                {xpos + w, ypos, 1.0, 1.0},

                {xpos, ypos + h, 0.0, 0.0},
                {xpos + w, ypos, 1.0, 1.0},
                {xpos + w, ypos + h, 1.0, 0.0} };
            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, pVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
        }
        //pIBO->unbind();
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        pShader.disable();
    }

    void TextLayer::updateCamera(int screenWidth, int screenHeight)
    {
        pWidth = static_cast<float>(screenWidth);
        pHeight = static_cast<float>(screenHeight);
        pShader.enable();
        pShader.setUniformMat4("projMatrix", glm::ortho(0.0f, pWidth, 0.0f, pHeight));
        pShader.disable();
    }

} // namespace Plutus