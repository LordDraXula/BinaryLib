#ifndef ENUMDEFINITIONS_H
#define ENUMDEFINITIONS_H

namespace BinaryLib {
    enum BinaryError {
        BIN_OK = 0,
        ERROR_NULL_POINTER = 1,
        ERROR_INVALID_ENDIAN = 2,
        ERROR_FILE = 3,
        ERROR_INVALID_REFLECTION = 4,
        ERROR_OUT_OF_BOUNDS = 5,
        ERROR_NOT_IMPL = 4000
    };

    enum BinaryType {
        BinConst = 0,
        BinDyn = 1
    };

    enum Endian {
        BigEndian = 0,
        LittleEndian = 1,
        Exception = 2
    };

    enum DataTypes {
        UINT8_B,
        UINT16_B,
        UINT32_B,
        UINT64_B,
        INT8_B,
        INT16_B,
        INT32_B,
        INT64_B,
        SINGLE_B,
        DOUBLE_B,
        BYTE_ARRAY
    };

    enum DTSM_Type {
        DTSMREF = 0,
        DTSMSTRUCT = 1,
        DTSMLIST = 2,
        DTSMSTRUCTLIST = 3
    };
}

#endif // ENUMDEFINITIONS_H
