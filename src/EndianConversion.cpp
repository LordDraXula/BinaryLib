#include "EndianConversion.h"
#include "Reflection.h"
#include "BinaryMember.h"

namespace BinaryLib {
    EndianConversion::EndianConversion() {}

    BinaryError EndianConversion::convertEndian(IBinary* const o_binary) {
        if(o_binary == nullptr) {
            return ERROR_NULL_POINTER;
        }

        if(o_binary->getActiveEndian() == Endian::Exception) {
            return ERROR_INVALID_ENDIAN;
        }

        IReflection* const reflection = o_binary->getDataReflection();
        if (reflection == nullptr) {
            return ERROR_NULL_POINTER;
        }


        uint8_t* data = nullptr;

        const BinaryError err = o_binary->getDataTable(&data);
        RETURN_ON_ERROR(err);

        if(data == nullptr) {
            return ERROR_NULL_POINTER;
        }
        if (reflection->_type() == BinaryType::BinConst) {

            uint64_t offset = 0;
            uint64_t reflectionIndex = 0;

            const uint64_t overallSize = reflection->_size();

            ConstDataReflection* cReflection = dynamic_cast<ConstDataReflection*>(reflection);
            while (offset < overallSize) {
                const DataTypeSizeMatch dtsm = cReflection->types[reflectionIndex++];

                convertEndian(&data[offset], dtsm);
                offset += dtsm.s;
            }
        }
        else {
            DynDataReflection* cReflection = dynamic_cast<DynDataReflection*>(reflection);

            uint64_t offset = 0;

            for (uint32_t reflectionIndex = 0; reflectionIndex < cReflection->indexSize; reflectionIndex++) {
                IDTSM* dtsm = cReflection->dtsmList[reflectionIndex];
                convertEndian(&data[offset], dtsm);

                switch (dtsm->_dtype()) {
                    case DTSMREF:
                    case DTSMSTRUCT: {
                        offset += dtsm->_size();
                        break;
                    }
                    case DTSMLIST: {
                        REF_List* ref = dynamic_cast<REF_List*>(dtsm);
                        offset += ref->type->_size();
                        break;
                    }
                    case DTSMSTRUCTLIST: {
                        STRUCT_List* ref = dynamic_cast<STRUCT_List*>(dtsm);
                        offset += ref->type->_size();
                        break;
                    }
                }
            }

        }

        switch (o_binary->getActiveEndian()) {
            case LittleEndian:
                o_binary->setActiveEndian(BigEndian);
                break;
            case BigEndian:
                o_binary->setActiveEndian(LittleEndian);
                break;
            default:
                return ERROR_INVALID_ENDIAN;
        }

        return BIN_OK;
    }

    BinaryError EndianConversion::convertEndian(uint8_t* const o_data, const uint8_t size) {
        if(o_data == nullptr) {
            return ERROR_NULL_POINTER;
        }
        if (size == 1) {
            return BIN_OK;
        }

        for (uint8_t i = 0; i < (size/2); i++) {
            const uint8_t holder = o_data[i];
            o_data[i] = o_data[(size - 1) - i];
            o_data[(size - 1) - i] = holder;
        }

        return BIN_OK;
    }

    BinaryError EndianConversion::convertEndian(uint8_t* const o_data, const DataTypeSizeMatch match) {
        if(o_data == nullptr) {
            return ERROR_NULL_POINTER;
        }

        if(match.t != BYTE_ARRAY) {
            return convertEndian(o_data, match.s & 0xFF);
        }

        return BIN_OK;
    }

    BinaryError EndianConversion::convertEndian(uint8_t* const o_data, const IDTSM* dtsm) {
        if(o_data == nullptr) {
            return ERROR_NULL_POINTER;
        }

        switch(dtsm->_dtype()) {
            case DTSMREF: {
                return convertEndian(o_data, dtsm->_size() & 0xFF);
            }
            case DTSMSTRUCT: {
                const DTSM_Struct* dtsmStruct = dynamic_cast<const DTSM_Struct*>(dtsm);
                uint64_t offset = 0;
                for (uint32_t index = 0; index < dtsmStruct->indexSize; index++) {
                    IDTSM* type = dtsmStruct->types[index];
                    convertEndian(&o_data[offset], type);
                    offset += type->_size();
                }
                break;
            }
            case DTSMLIST: {
                const REF_List* dtsmList = dynamic_cast<const REF_List*>(dtsm);
                const uint8_t iterationSize = dtsmList->type->s & 0xFF;

                if (iterationSize == 1) {
                    return BIN_OK;
                }

                BinaryListAbsolute<uint8_t, DTSM_Ref>* binData = reinterpret_cast<BinaryListAbsolute<uint8_t, DTSM_Ref>*>(o_data);
                uint8_t* listData = nullptr;
                binData->_data(&listData);

                const uint64_t dataSize = dtsmList->_size();

                for (uint32_t index = 0; index < dataSize; index += iterationSize) {
                    convertEndian(&listData[index], iterationSize);
                }
                break;
            }
            case DTSMSTRUCTLIST: {
                const STRUCT_List* structList = dynamic_cast<const STRUCT_List*>(dtsm);
                const uint64_t iterationSize = structList->type->_size();

                if (iterationSize == 1) {
                    return BIN_OK;
                }

                BinaryListAbsolute<uint8_t, DTSM_Struct>* binData = reinterpret_cast<BinaryListAbsolute<uint8_t, DTSM_Struct>*>(o_data);
                uint8_t* listData = nullptr;
                binData->_data(&listData);

                const uint64_t dataSize = structList->_size();

                for (uint32_t index = 0; index < dataSize; index += iterationSize) {
                    convertEndian(&listData[index], structList->type);
                }
                break;
            }
        }

        return BIN_OK;
    }

    Endian EndianConversion::getSystemEndian() {
        uint8_t* a;
        uint16_t d = 0x37BF;
        a = reinterpret_cast<uint8_t*>(&d);

        if (a[0] == 0x37 & a[1] == 0xBF) {
            return BigEndian;
        }
        else if (a[0] == 0xBF & a[1] == 0x37) {
            return LittleEndian;
        }

        return Endian::Exception;
    }
}
