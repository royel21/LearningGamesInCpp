#include "AssetManager.h"

namespace Plutus
{
	AssetManager2* AssetManager2::get()
	{
		static AssetManager2 instance;
		return &instance;
	}

	void AssetManager2::destroy() {
		for (auto& repo : mAssets) {
			for (auto& a : repo.second) {
				a.second->destroy();
				delete a.second;
			}
		}
		mAssets.clear();
	}
}