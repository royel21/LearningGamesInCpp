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

    struct TileSet
    {
        std::string name;
        std::string path;
        int columns;
        int tileWidth;
        int tileHeight;
        int totalTiles;
        std::vector<glm::vec4> uvs;
        int mTexCount = 0;
        GLTexture mTexture;

        TileSet() : columns(0), tileWidth(0), tileHeight(0) {}

        /***
		 *Create a texture atlas from the image from the tile width and height
		 * @param id the Id of the texture
		 * @param c count/total tile in the altas
		 * @param tex reference to the texture
		 * **/
        TileSet(const std::string &id, int c, int w, int h, GLTexture tex, const std::string &_path);

        inline const glm::vec4 getUV(int texcoord)
        {
            return texcoord < mTexCount ? uvs[texcoord] : glm::vec4();
        }

    private:
        void calculateUV();
    };

    class Textures
    {
    public:
        std::unordered_map<std::string, TileSet> mTileSets;

    public:
        static Textures *get();
        ~Textures();
        TileSet *getTexture(const std::string &id) { return &mTileSets[id]; }

        const TileSet *addTexture(const std::string &id, const std::string &path, GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);
        const TileSet *addTexture(const std::string &id, const std::string &path, int c, int w, int h, GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);
        void removeTexture(const std::string &id);

        void cleanUp();

    private:
        Textures() = default;
        GLTexture loadTexture(std::string path, GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);
        std::unordered_map<std::string, bool> mTextureMap;
    };

} // namespace Plutus
