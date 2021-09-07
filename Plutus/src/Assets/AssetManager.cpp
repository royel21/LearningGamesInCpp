#include "AssetManager.h"
#include <algorithm>
#include <sstream>
#include "Textures.h"

namespace Plutus
{
	AssetManager::AssetManager()
	{
	}
	AssetManager* AssetManager::get()
	{
		static AssetManager instance;
		return &instance;
	}

	AssetManager::~AssetManager()
	{
		clearData();
	}

	void AssetManager::clearData()
	{
		mTextures.cleanUp();
		mFonts.cleanUp();
	}
} // namespace Plutus
