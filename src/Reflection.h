#ifndef REFLECTION_H
#define REFLECTION_H

#include "DTSM.h"

namespace BinaryLib {
    struct ConstDataReflection : public BinStruct, IReflection
    {
        ConstDataReflection(const DataTypeSizeMatch* types, const uint32_t indexSize);
        ConstDataReflection(const ConstDataReflection& other);

        uint64_t _size() const override;
        uint64_t _offset(const uint32_t index) const override;
        BinaryType _type() const override;
    };

    struct DynDataReflection : public IReflection
    {
        IDTSM** dtsmList;
        uint32_t indexSize;

        DynDataReflection(IDTSM** dtsmList, const uint32_t indexSize);

        virtual ~DynDataReflection() override;


        uint64_t find(const IDTSM* const ref) const;

        uint64_t _size() const override;
        uint64_t _offset(const uint32_t index) const override;
        BinaryType _type() const override;
    };
}

#endif // REFLECTION_H
