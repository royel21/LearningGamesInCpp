#include "SceneAsset.h"
#include <Utils/FileIO.h>

namespace Plutus
{
    void SceneAsset::init(const std::string& path)
    {
        mPath = path;
        mBuffer = FileIO::readFileAsString((baseDir + mPath).c_str());
    }

    void SceneAsset::save()
    {
        FileIO::saveBufferToFile((baseDir + mPath), mBuffer.c_str());
    }

    void SceneAsset::destroy()
    {
        mBuffer = "";
    }
}