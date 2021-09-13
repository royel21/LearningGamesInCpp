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

		GLuint getTexId(const std::string& id) {
			return mTextures.getTexture(id)->texId;
		}

		glm::vec4 getTexCoords(const std::string& id, int index) {
			return mTextures.getTexture(id)->getUV(index);
		}

	private:
		AssetManager();
	};
} // namespace Plutus

#endif