#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <Math/Vectors.h>
#include "Graphics/GLheaders.h"

namespace Plutus
{
    struct GLTexture
    {
        GLTexture() : id(0), width(0), height(0) {}
        GLTexture(uint32_t _id, int _width, int _height) : id(_id), width(_width), height(_height) {}
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
        uint32_t texId = 0;
        std::vector<vec4f> uvs;

        Texture() : columns(0), tileWidth(0), tileHeight(0) {}

        /***
         *Create a texture atlas from the image from the tile width and height
         * @param id the Id of the texture
         * @param columns count/total tile in the altas
         * @param tex reference to a texture
         * **/
        Texture(const std::string& id, int columns, int w, int h, GLTexture tex, const std::string& _path);

        vec4f getUV(float column, float row, float w, float h);

        vec4f getUV(int texcoord)
        {
            return uvs.size() && texcoord < (int)uvs.size() ? uvs[texcoord] : vec4f(0, 0, 1, 1);
        }

        void calculateUV();
    };

    class Textures
    {
    public:
        std::unordered_map<std::string, Texture> mTileSets;

    public:
        Textures() = default;
        ~Textures();

        Texture* getTexture(const std::string& id);
        uint32_t getTextureId(const std::string& id);
        vec4f getTextureUV(const std::string& id, int uvIndex);

        std::unordered_map<std::string, Texture>& getItems() { return mTileSets; }

        void remove(std::string texture);
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
        GLTexture createGLTexure(std::string path, GLint minFilter = GL_NEAREST, GLint magFilter = GL_NEAREST);
    private:
        std::unordered_map<std::string, bool> mTextureMap;
    };

} // namespace Plutus
