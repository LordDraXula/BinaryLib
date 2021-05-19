#include "Reflection.h"

namespace BinaryLib {
    ConstDataReflection::ConstDataReflection(const DataTypeSizeMatch* types, const uint32_t indexSize)
        : BinStruct (types, indexSize)
    {}

    ConstDataReflection::ConstDataReflection(const ConstDataReflection& other)
        : BinStruct (other)
    {}

    uint64_t ConstDataReflection::_size() const {
        return BinStruct::_size();
    }

    uint64_t ConstDataReflection::_offset(const uint32_t index) const {
        return BinStruct::_offset(index);
    }

    BinaryType ConstDataReflection::_type() const {
        return BinaryType::BinConst;
    }


    DynDataReflection::DynDataReflection(IDTSM** dtsmList, const uint32_t indexSize)
        : indexSize(indexSize)
    {
        this->dtsmList = dtsmList;
    }

    DynDataReflection::~DynDataReflection() {
        delete [] dtsmList;
        dtsmList = nullptr;
    }

    uint64_t DynDataReflection::find(const IDTSM* const ref) const {
        uint64_t offset = 0;

        for (uint32_t index = 0; index < indexSize; index++) {
            IDTSM* cur = dtsmList[index];

            const uint64_t res = cur->find(ref);
            offset += res;
            if (res < cur->_size()) {
                return offset;
            }
        }

        return offset;
    }

    uint64_t DynDataReflection::_size() const {
        return _offset(indexSize);
    }

    uint64_t DynDataReflection::_offset(const uint32_t index) const {
        uint64_t size = 0;

        for (uint32_t i =0; i < index; i++) {
            size += dtsmList[i]->_size();
        }

        return size;
    }

    BinaryType DynDataReflection::_type() const {
        return BinaryType::BinDyn;
    }
}
