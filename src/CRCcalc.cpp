#include "CRCcalc.h"

#include "EndianConversion.h"

namespace BinaryLib {
    CRCcalc::CRCcalc() {}

    uint32_t CRCcalc::CRC32(const uint8_t* const data, const uint64_t size, const uint64_t ignoreIndex) {
        uint32_t crc32 = 0xFFFFFFFF;

        if (data == nullptr) {
            return crc32;
        }

        for (uint32_t index = 0; index < size; index++) {
            if(ignoreIndex == index) {
                index += 3;
                continue;
            }
            const uint8_t lookupIndex = (crc32 ^ data[index]) & 0xFF;
            crc32 = (crc32 >> 8) ^ CRC32_Table[lookupIndex];
        }

        return (crc32 ^ 0xFFFFFFFF);
    }

    uint32_t CRCcalc::updateCRC32constOffset(IBinary* const binData, const uint64_t crcOffset) {
        if (binData == nullptr) {
            return 0xFFFFFFFF;
        }

        IReflection* reflection = binData->getDataReflection();
        if (reflection == nullptr) {
            return ERROR_NULL_POINTER;
        }

        uint8_t* data = nullptr;
        if (reflection->_type() == BinaryType::BinConst) {
            if (binData->getDataTable(&data) != BIN_OK) {
                return 0xFFFFFFFF;
            }
        }
        else {

        }


        if (data == nullptr) {
            return 0xFFFFFFFF;
        }

        const uint64_t size = binData->getDataReflection()->_size();


        uint32_t crc32 = CRC32(data, size, crcOffset);
        if(binData->getActiveEndian() != EndianConversion::getSystemEndian()) {
            BinaryError err = EndianConversion::convertEndian(reinterpret_cast<uint8_t*>(&crc32), DataTypeSizeMatch(UINT32_B));
            RETURN_ON_ERROR(err);
        }

        if (reflection->_type() == BinaryType::BinConst) {
            memcpy(data + crcOffset, &crc32, sizeof(uint32_t));
        }
        else {

        }

        return crc32;
    }

    uint32_t CRCcalc::updateCRC32(IBinary* const binData, const uint32_t reflectionIndex) {
        return updateCRC32constOffset(binData, binData->getDataReflection()->_offset(reflectionIndex));
    }
}
