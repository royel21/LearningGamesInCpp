#include "Script.h"
#include <Utils/FileIO.h>

namespace Plutus
{
    void Script::init(const std::string& path)
    {
        mPath = path;
        mBuffer = FileIO::readFileAsString((baseDir + mPath).c_str());
    }

    void Script::save()
    {
        FileIO::saveBufferToFile((baseDir + mPath), mBuffer.c_str());
    }

    void Script::destroy()
    {
        mBuffer = "";
    }
}
