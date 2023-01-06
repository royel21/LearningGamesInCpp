#include "Script.h"
#include <Utils/FileIO.h>

namespace Plutus
{
    void Script::init(const std::string& path)
    {
        if (!FileIO::exists(path)) {
            save();
        }
        mPath = path;
        fullPath = baseDir + mPath;
        mBuffer = FileIO::readFileAsString(fullPath.c_str());
    }

    void Script::save(const std::string& buffer)
    {
        if (!buffer.empty()) {
            mBuffer = buffer;
        }
        FileIO::saveBufferToFile(fullPath, mBuffer.c_str());
    }

    void Script::destroy()
    {
        mBuffer = "";
    }

    std::string Script::load() {
        mBuffer = FileIO::readFileAsString(fullPath.c_str());
        return  mBuffer;
    }
}
