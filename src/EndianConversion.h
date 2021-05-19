#ifndef ENDIANCONVERSION_H
#define ENDIANCONVERSION_H

#include "IBinary.h"

namespace BinaryLib {
    class EndianConversion
    {
    private:
        EndianConversion();

    public:
        static BinaryError convertEndian(IBinary* const o_binary);
        static BinaryError convertEndian(uint8_t* const o_data, const uint8_t size);
        static BinaryError convertEndian(uint8_t* const o_data, const DataTypeSizeMatch match);
        static BinaryError convertEndian(uint8_t* const o_data, const IDTSM* dtsm);

        static Endian getSystemEndian();
    };
}

#endif // ENDIANCONVERSION_H
