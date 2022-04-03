#include "FileIO.h"
#include <cstdio>
#include <filesystem>

#pragma warning(disable : 4996)

namespace Plutus
{
    std::vector<unsigned char> readFile(const char* path2File, const char* mode)
    {
        FILE* fp = fopen(path2File, mode);
        std::vector<unsigned char> vec;

        if (fp != NULL)
        {
            /* Go to the end of the file. */
            if (fseek(fp, 0L, SEEK_END) == 0)
            {
                /* Get the size of the file. */
                unsigned long size = ftell(fp);
                if (size == -1)
                { /* Error */
                    fclose(fp);
                    return vec;
                }
                vec.resize(size);

                rewind(fp);
                /* Read the entire file into memory. */
                fread(vec.data(), sizeof(char), size, fp);
            }
            fclose(fp);
        }
        return vec;
    }

    std::string readFileAsString(const char* path)
    {
        auto data = readFile(path, "rb");
        return data.size() ? std::string(reinterpret_cast<char*>(data.data()), data.size()) : "";
    }

    bool exists(const char* path)
    {
        return std::filesystem::exists(path);
    }
}