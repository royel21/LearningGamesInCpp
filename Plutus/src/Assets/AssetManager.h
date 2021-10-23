#ifndef _ASSETMANAGER_H
#define _ASSETMANAGER_H

#include <unordered_map>
#include <string>
#include <vector>

#include "Textures.h"
#include "FontManager.h"

namespace Plutus
{
	class AssetManager
	{
	public:
		Textures mTextures;
		FontManager mFonts;

	public:
		static AssetManager* get();

		~AssetManager();
		void clearData();

		GLuint getTexId(const std::string& id)
		{
			return mTextures.getTextureId(id);
		}

		vec4f getTexCoords(const std::string& id, int index)
		{
			return mTextures.getTextureUV(id, index);
		}

		vec4f getTexCoords(const std::string& id, vec4f coords)
		{
			return mTextures.getTexture(id)->getUV(coords.x, coords.y, coords.z, coords.w);
		}

	private:
		AssetManager();
	};
} // namespace Plutus

#endif