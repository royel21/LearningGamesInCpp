#include "AssetManager.h"
#include <Platforms/SoundEngine.h>


namespace Plutus
{
	AssetManager* AssetManager::get()
	{
		static AssetManager instance;
		return &instance;
	}

	void AssetManager::destroy() {
		for (auto& repo : mAssets) {
			for (auto& a : repo.second) {
				a.second->destroy();
				delete a.second;
			}
		}
		SoundEngine.cleanUp();
		mAssets.clear();
	}
}