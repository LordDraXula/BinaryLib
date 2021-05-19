#ifndef IDEFINITIONS_H
#define IDEFINITIONS_H

#include <cstdint>
#include "EnumDefinitions.h"

namespace BinaryLib {

    struct IBinLink
    {
    protected:
        IBinLink();
    public:
        virtual ~IBinLink();

        virtual uint64_t _size() const;
        virtual uint64_t _offset(const uint32_t index) const;
    };

    struct IDTSM : public IBinLink
    {
    protected:
        IDTSM();

    public:
        virtual ~IDTSM();

        virtual DTSM_Type _dtype() const;

        virtual uint64_t find(const IDTSM* const ref) const;
    };

    struct IReflection : public IBinLink
    {
    protected:
         IReflection();

    public:
        virtual ~IReflection();

        virtual BinaryType _type() const = 0;
    };
}

#endif // IDEFINITIONS_H
