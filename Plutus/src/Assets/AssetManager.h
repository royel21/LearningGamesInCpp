#ifndef _ASSETMANAGER_H
#define _ASSETMANAGER_H

#include <unordered_map>
#include <string>
#include <vector>
#include "glm/glm.hpp"

namespace Plutus
{
	class Textures;
	//Manage all the texture of the game
	class AssetManager
	{
	public:
		Textures* mTextures = nullptr;

	public:
		static AssetManager* get();
		~AssetManager();
		void clearData();

	private:
		AssetManager();
	};
} // namespace Plutus

#endif