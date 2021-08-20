#include "AssetManager.h"
#include <algorithm>
#include <sstream>
#include "Textures.h"

namespace Plutus
{
	AssetManager::AssetManager()
	{
		mTextures = Textures::get();
	}
	AssetManager* AssetManager::get()
	{
		static AssetManager instance;
		return &instance;
	}

	AssetManager::~AssetManager()
	{
		mTextures->cleanUp();
	}

	void AssetManager::clearData()
	{
		mTextures->cleanUp();
	}
} // namespace Plutus
