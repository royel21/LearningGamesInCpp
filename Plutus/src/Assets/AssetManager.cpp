#include "AssetManager.h"
#include <Platforms/SoundEngine.h>

#include <Log/Logger.h>
#include <miniaudio.h>

namespace Plutus
{
	AssetManager* AssetManager::get()
	{
		static AssetManager instance;
		return &instance;
	}

	AssetManager::AssetManager() {
		if (mAudioEngine == nullptr) {
			mAudioEngine = new ma_engine;

			ma_result result = ma_engine_init(NULL, mAudioEngine);

			if (result != MA_SUCCESS) {
				delete mAudioEngine;
				mAudioEngine = nullptr;
				Logger::error("Failed to initialize audio engine.");

			}
		}
	}

	AssetManager::~AssetManager() {
		destroy();

		if (mAudioEngine != nullptr) {
			ma_engine_uninit(mAudioEngine);
			delete mAudioEngine;
			mAudioEngine = nullptr;
		}
	}

	void AssetManager::destroy() {
		for (auto& repo : mAssets) {
			for (auto& a : repo.second) {
				a.second->destroy();
				delete a.second;
			}
		}
		mAssets.clear();
	}
}