#ifndef IOBJECT_H
#define IOBJECT_H

#include "Definitions.h"

namespace BinaryLib {
    class IBinary {
    protected:
        IBinary();

    public:
        virtual ~IBinary();

        virtual IReflection* getDataReflection() = 0;

        virtual Endian getActiveEndian() const = 0;
        virtual void setActiveEndian(const Endian endian) = 0;

        virtual BinaryError getDataTable(uint8_t** o_data) = 0;
    };
}


#endif // IOBJECT_H
