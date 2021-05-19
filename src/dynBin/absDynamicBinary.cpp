#include "absDynamicBinary.h"

#include "../EndianConversion.h"

namespace BinaryLib {
    absDynamicBinary::absDynamicBinary(DynDataReflection* reflection, const Endian endian)
        : absBinary (reflection, endian)
    {
    }

    BinaryError absDynamicBinary::handleInitSection(IDTSM* currentDTSM, const uint8_t** data, const uint8_t* const dataCopy, uint8_t** structData) {
        switch (currentDTSM->_dtype()) {
            case DTSMREF:
            case DTSMSTRUCT: {
                uint64_t size(currentDTSM->_size());

                memcpy(*structData, *data, size);
                *structData += size;
                *data += size;
                break;
            }
            case DTSMLIST: {
                REF_List* list = static_cast<REF_List*>(currentDTSM);
                const uint64_t typeOffset = static_cast<DynDataReflection*>(reflection)->find(list->indexRef);
                const uint32_t typeSize = list->indexRef->s;

                uint8_t* s = new uint8_t[typeSize];
                memcpy(s, &dataCopy[typeOffset], typeSize);

                if (EndianConversion::getSystemEndian() != endian) {
                    EndianConversion::convertEndian(s, *list->indexRef);
                }

                uint32_t c = cUtilUn(s, typeSize & 0xFF).u32;

                BinaryListAbsolute<uint8_t, DTSM_Ref>* representationList = reinterpret_cast<BinaryListAbsolute<uint8_t, DTSM_Ref>*>(*structData);
                BinaryError err = representationList->init(list, *data, c * list->type->_size(), c);
                RETURN_ON_ERROR(err);

                delete [] s;
                s = nullptr;

                *structData += sizeof(BinaryListAbsolute<uint8_t, DTSM_Ref>);
                *data += currentDTSM->_size();

                break;
            }
            case DTSMSTRUCTLIST: {
                STRUCT_List* list = static_cast<STRUCT_List*>(currentDTSM);
                const uint64_t typeOffset = static_cast<DynDataReflection*>(reflection)->find(list->indexRef);
                const uint32_t typeSize = list->indexRef->s;

                uint8_t* s = new uint8_t[typeSize];
                memcpy(s, &dataCopy[typeOffset], typeSize);

                if (EndianConversion::getSystemEndian() != endian) {
                    EndianConversion::convertEndian(s, *list->indexRef);
                }

                uint32_t c = cUtilUn(s, typeSize & 0xFF).u32;

                BinaryListAbsolute<uint8_t, DTSM_Struct>* representationList = reinterpret_cast<BinaryListAbsolute<uint8_t, DTSM_Struct>*>(*structData);
                BinaryError err = representationList->init(list, *data, c * list->type->_size(), c);
                RETURN_ON_ERROR(err);
                delete [] s;
                s = nullptr;

                *structData += sizeof(BinaryListAbsolute<uint8_t, DTSM_Struct>);
                *data += currentDTSM->_size();

                break;
            }
        }

        return BIN_OK;
    }

    BinaryError absDynamicBinary::init(const uint8_t* data) {
        DynDataReflection* initReflection = static_cast<DynDataReflection*>(reflection);

        IDTSM** reflList = initReflection->dtsmList;
        const uint32_t listSize = initReflection->indexSize;

        const uint8_t* const dataCopy = data;
        uint8_t* structData = nullptr;
        BinaryError err = getDataTable(&structData);
        RETURN_ON_ERROR(err);

        for (uint32_t index = 0; index < listSize; index++) {
            IDTSM* currentDTSM = reflList[index];
            err = handleInitSection(currentDTSM, &data, dataCopy, &structData);
            RETURN_ON_ERROR(err);
        }

        return BIN_OK;
    }

    BinaryError absDynamicBinary::getFullDataTable(uint8_t** o_data) {
        DynDataReflection* dynReflection = static_cast<DynDataReflection*>(reflection);
        *o_data = new uint8_t[dynReflection->_size()];
        uint8_t* modData = *o_data;

        IDTSM** reflList = dynReflection->dtsmList;
        const uint32_t listSize = dynReflection->indexSize;

        uint8_t* structData = nullptr;
        BinaryError err = getDataTable(&structData);
        RETURN_ON_ERROR(err);

        for (uint32_t index = 0; index < listSize; index++) {
            IDTSM* currentDTSM = reflList[index];
            uint64_t size = currentDTSM->_size();

            switch (currentDTSM->_dtype()) {
                case DTSMREF:
                case DTSMSTRUCT: {
                    memcpy(modData, structData, size);

                    modData += size;
                    structData += size;
                    break;
                }
                case DTSMLIST: {
                    BinaryListAbsolute<uint8_t, DTSM_Ref>* rep = reinterpret_cast<BinaryListAbsolute<uint8_t, DTSM_Ref>*>(structData);
                    uint8_t* dtsmData = nullptr;
                    rep->_data(&dtsmData);

                    memcpy(modData, dtsmData, size);

                    modData += size;
                    structData += sizeof(*rep);
                    break;
                }
                case DTSMSTRUCTLIST: {
                    BinaryListAbsolute<uint8_t, DTSM_Struct>* rep = reinterpret_cast<BinaryListAbsolute<uint8_t, DTSM_Struct>*>(structData);
                    uint8_t* dtsmData = nullptr;
                    rep->_data(&dtsmData);

                    memcpy(modData, dtsmData, size);

                    modData += size;
                    structData += sizeof(*rep);
                    break;
                }
            }
        }

        return BIN_OK;
    }
}
