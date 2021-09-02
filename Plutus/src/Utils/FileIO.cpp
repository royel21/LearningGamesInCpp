#include "FileIO.h"
#include "cstdio"

#pragma warning(disable:4996)

namespace Plutus
{
    std::vector<unsigned char> readFile(char* path2File, char* mode)
    {
        FILE* fp = fopen(path2File, mode);
        std::vector<unsigned char> vec;

        if (fp != NULL) {
            /* Go to the end of the file. */
            if (fseek(fp, 0L, SEEK_END) == 0) {
                /* Get the size of the file. */
                unsigned long size = ftell(fp);
                if (size == -1) { /* Error */ }
                vec.resize(size);
                /* Allocate our buffer to that size. */

                /* Go back to the start of the file. */
                if (fseek(fp, 0L, SEEK_SET) != 0) { /* Error */ }

                /* Read the entire file into memory. */
                size_t newLen = fread(vec.data(), sizeof(char), size, fp);
            }
            fclose(fp);
        }
        return vec;
    }
}