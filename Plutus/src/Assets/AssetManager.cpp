#include "AssetManager.h"
#ifdef __EMSCRIPTEN__
#include <Platforms/Web/AudioEngine.h>
#elif __ANDROID__
#include <Platforms/Android/AudioEngine.h>
#else
#include <Platforms/Windows/AudioEngine.h>
#include <Platforms/Windows/AudioEvent.h>
#endif


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
		SoundEngine.cleanUp();
		mAssets.clear();
	}
}