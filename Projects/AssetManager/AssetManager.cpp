#include "AssetManager.h"

namespace Plutus
{
	AssetManager2* AssetManager2::get()
	{
		static AssetManager2 instance;
		return &instance;
	}
}