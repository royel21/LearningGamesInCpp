#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Graphics/GLheaders.h"

namespace Plutus
{
    struct GLTexture
    {
        GLTexture() : id(0), width(0), height(0) {}
        uint32_t id;
        int width;
        int height;
    };

    struct Texture
    {
        std::string name;
        std::string path;
        int columns = 0;
        int tileWidth = 0;
        int tileHeight = 0;
        int texWidth = 0;
        int texHeight = 0;
        uint32_t texId = -1;
        std::vector<glm::vec4> uvs;

        Texture() : columns(0), tileWidth(0), tileHeight(0) {}

        /***
         *Create a texture atlas from the image from the tile width and height
         * @param id the Id of the texture
         * @param columns count/total tile in the altas
         * @param tex reference to a texture
         * **/
        Texture(const std::string& id, int columns, int w, int h, GLTexture tex, const std::string& _path);

        glm::vec4 getUV(float column, float row, float w, float h);

        glm::vec4 getUV(int texcoord)
        {
            return uvs[texcoord];
        }

    private:
        void calculateUV();
    };

    class Textures
    {
    public:
        std::unordered_map<std::string, Texture> mTileSets;

    public:
        Textures() = default;
        ~Textures();
        Texture* getTexture(const std::string& id) { return &mTileSets[id]; }
        glm::vec4 getTextureUV(const std::string& id, int uvIndex) { return mTileSets[id].getUV(uvIndex); }

        std::unordered_map<std::string, Texture>& getItems() { return mTileSets; }

        void removeItem(std::string texture);
        /***
            Create a texture atlas from the image from the tile width and height
            @param id the Id of the texture
            @param path route to file
        ***/
        const Texture* addTexture(const std::string& id, const std::string& path, GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);
        /***
           *Create a texture atlas from the image from the tile width and height
            @param id the Id of the texture
            @param path route to file
       ***/
        const Texture* addTexture(const std::string& id, const std::string& path, int c, int w, int h, GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);
        void removeTexture(const std::string& id);

        void cleanUp();

    private:
        GLTexture loadTexture(std::string path, GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);
        std::unordered_map<std::string, bool> mTextureMap;
    };

} // namespace Plutus
