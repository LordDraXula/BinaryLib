#include "BinaryFileHandler.h"

#include <fstream>
#include <iterator>
#include <vector>

namespace BinaryLib {
    BinaryFileHandler::BinaryFileHandler()
    {

    }

    uint64_t BinaryFileHandler::readFile(uint8_t** o_data, const std::string& filePath) {
        if(o_data == nullptr) {
            return 0;
        }

        std::ifstream input(filePath, std::ios::binary);
        std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(input), {});

        const uint64_t size = buffer.size();

        *o_data = new uint8_t[size];
        memcpy(*o_data, buffer.data(), size * sizeof(uint8_t));

        input.close();

        return size;
    }

    BinaryError BinaryFileHandler::writeFile(IBinary* const binData, const std::string& filePath) {
        if (binData == nullptr) {
            return ERROR_NULL_POINTER;
        }

        IReflection* reflection = binData->getDataReflection();
        if (reflection == nullptr) {
            return ERROR_NULL_POINTER;
        }

        uint8_t* data = nullptr;
        if (reflection->_type() == BinaryType::BinConst) {
            const BinaryError err = binData->getDataTable(&data);
            RETURN_ON_ERROR(err);
        }
        else {

        }

        if (data == nullptr) {
            return ERROR_NULL_POINTER;
        }
        const uint64_t size = reflection->_size();

        return writeFile(data, size, filePath);
    }

    BinaryError BinaryFileHandler::writeFile(const uint8_t* const data, const uint64_t size, const std::string& filePath) {
        if (data == nullptr) {
            return BinaryError::ERROR_NULL_POINTER;
        }

        std::ofstream output(filePath, std::ios::binary);

        output.write(reinterpret_cast<const char*>(data), size);

        output.close();

        return BinaryError::BIN_OK;
    }
}

