#include "DTSM.h"

namespace BinaryLib {
    DataTypeSizeMatch::DataTypeSizeMatch(const DataTypes t, const uint32_t dataSize)
        : t(t)
    {
        switch (t) {
            case UINT8_B:
            case INT8_B:
                s = sizeof(uint8_t);
                break;
            case UINT16_B:
            case INT16_B:
                s = sizeof(uint16_t);
                break;
            case UINT32_B:
            case INT32_B:
            case SINGLE_B:
                s = sizeof(uint32_t);
                break;
            case UINT64_B:
            case INT64_B:
            case DOUBLE_B:
                s = sizeof(uint64_t);
                break;
            case BYTE_ARRAY:
                s = dataSize;
                break;
        }
    }

    //############################################################################

    BinStruct::BinStruct(const DataTypeSizeMatch* types, const uint32_t indexSize)
        : indexSize(indexSize)
    {
        this->types = new DataTypeSizeMatch[indexSize];
        memcpy(this->types, types, indexSize * sizeof (DataTypeSizeMatch));
    }

    BinStruct::BinStruct(const BinStruct& other)
        : indexSize(other.indexSize)
    {
        this->types = new DataTypeSizeMatch[indexSize];
        memcpy(this->types, other.types, indexSize * sizeof(DataTypeSizeMatch));
    }

    BinStruct::~BinStruct() {
        delete [] types;
        types = nullptr;
    }

    uint64_t BinStruct::_size() const {
        return _offset(indexSize);
    }

    uint64_t BinStruct::_offset(const uint32_t index) const {
        uint64_t offset = 0;

        for (uint32_t i = 0; i < index; i++) {
            offset += types[i].s;
        }

        return offset;
    }

    //############################################################################

    DTSM_Ref::DTSM_Ref(const DataTypes t, const uint32_t dataSize)
        : DataTypeSizeMatch (t, dataSize)
    {}

    DTSM_Ref::~DTSM_Ref()
    {}

    uint64_t DTSM_Ref::_size() const {
        return s;
    }

    uint64_t DTSM_Ref::_offset(const uint32_t index) const {
        return index > 0 ? s : 0;
    }

    DTSM_Type DTSM_Ref::_dtype() const {
        return DTSM_Type::DTSMREF;
    }

    uint64_t DTSM_Ref::find(const IDTSM *const ref) const {
        if (this == ref) {
            return 0;
        }

        return s;
    }

    //############################################################################

    DTSM_Struct::DTSM_Struct(IDTSM** types, const uint32_t indexSize)
        : indexSize(indexSize)
    {
        this->types = types;
    }

    DTSM_Struct::~DTSM_Struct() {
        delete [] types;
        types = nullptr;
    }

    uint64_t DTSM_Struct::_size() const {
        return _offset(indexSize);
    }

    uint64_t DTSM_Struct::_offset(const uint32_t index) const {
        uint32_t offset = 0;

        for (uint32_t i = 0; i < index; i++) {
            offset += types[i]->_size();
        }

        return offset;
    }

    DTSM_Type DTSM_Struct::_dtype() const {
        return DTSM_Type::DTSMSTRUCT;
    }

    uint64_t DTSM_Struct::find(const IDTSM *const ref) const {
        if (ref == this) {
            return 0;
        }

        uint32_t offset = 0;

        for (uint32_t index = 0; index < indexSize; index++) {
            const uint64_t res = types[index]->find(ref);
            offset += res;

            if (res < types[index]->_size()) {
                return offset;
            }
        }

        return offset;
    }

    //############################################################################

    DTSM_Factory::DTSM_Map DTSM_Factory::storage;

    DTSM_Factory::DTSM_Factory() {}
    DTSM_Factory::~DTSM_Factory() {
        storage.clear();
    }

    DTSM_Ref* DTSM_Factory::_ref(const std::string key, const DataTypes dataType) {
        DTSM_Map::iterator it = storage.find(key);
        DTSM_Ref* ref = nullptr;

        if (it != storage.end()) {
            ref = it->second;
        } else {
            ref = new DTSM_Ref(dataType);
            storage[key] = ref;
        }

        return ref;
    }
}
