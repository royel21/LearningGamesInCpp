#include "AssetManager.h"
#include <algorithm>
#include <sstream>
#include "Serialize/Serialize.h"
#include "Textures.h"

namespace Plutus
{
	AssetManager::AssetManager()
	{
		mTextures = Textures::get();
	}
	AssetManager *AssetManager::get()
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

	void AssetManager::Serialize(Serializer &serializer)
	{
		auto writer = serializer.getWriter();
		writer->String("textures");
		writer->StartArray();
		for (auto tile : mTextures->mTileSets)
		{
			writer->StartObject();

			writer->String("id");
			writer->String(tile.first.c_str());
			writer->String("path");
			writer->String(tile.second.path.c_str());
			writer->String("columns");
			writer->Int(tile.second.columns);
			writer->String("width");
			writer->Int(tile.second.tileWidth);
			writer->String("height");
			writer->Int(tile.second.tileHeight);

			writer->EndObject();
		}
		writer->EndArray();
	}

} // namespace Plutus
