#ifndef BINARYFILEHANDLER_H
#define BINARYFILEHANDLER_H

#include "Definitions.h"
#include "IBinary.h"
#include <string>

const char pathSeparator =
#ifdef _WIN32
                            '\\';
#else
                            '/';
#endif

namespace BinaryLib {
    class BinaryFileHandler
    {
    private:
        BinaryFileHandler();
    public:

        static uint64_t readFile(uint8_t** o_data, const std::string& filePath);
        static BinaryError writeFile(IBinary* const binData, const std::string& filePath);

        static BinaryError writeFile(const uint8_t* const data, const uint64_t size, const std::string& filePath);
    };
}


#endif // BINARYFILEHANDLER_H
