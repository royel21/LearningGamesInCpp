#include "AssetManager.h"

namespace Plutus
{
	AssetManager2* AssetManager2::get()
	{
		static AssetManager2 instance;
		return &instance;
	}

	void AssetManager2::destroy() {
		for (auto& a : mAssets) {
			a.second->destroy();
			delete a.second;
		}
		mAssets.clear();
	}
}