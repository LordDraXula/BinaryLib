#ifndef MACROMAGE_H
#define MACROMAGE_H

#include "map.h"

#define MCOUNTER(var) 0
#define NUMARGS(...) (sizeof((uint8_t[]){__VA_ARGS__})/sizeof(uint8_t))

#define ONE_ARG(arg1, ...) arg1

#define PAIR1(var1, var2) var1
#define PAIR2(var1, var2) var2

#define MSTRUCT_VAR(typepair, name, ...) PAIR1 typepair name __VA_OPT__([ONE_ARG(__VA_ARGS__)])
#define MSTRUCT(var) MSTRUCT_VAR var;

#define MTYPE_VAR(typepair, name, ...) DataTypeSizeMatch(PAIR2 typepair __VA_OPT__(, ONE_ARG(__VA_ARGS__)))
#define MTYPES(var) MTYPE_VAR var



#define MUINT8 uint8_t, DataTypes::UINT8_B
#define MUINT16 uint16_t, DataTypes::UINT16_B
#define MUINT32 uint32_t, DataTypes::UINT32_B
#define MUINT64 uint64_t, DataTypes::UINT64_B
#define MINT8 int8_t, DataTypes::INT8_B
#define MINT16 int16_t, DataTypes::INT16_B
#define MINT32 int32_t, DataTypes::INT32_B
#define MINT64 int64_t, DataTypes::INT64_B
#define MSINGLE float, DataTypes::SINGLE_B
#define MDOUBLE double, DataTypes::DOUBLE_B
#define MBYTEARRAY uint8_t, DataTypes::BYTE_ARRAY

#define MVAR(type, name, ...) ((type), name __VA_OPT__(, ONE_ARG(__VA_ARGS__)))

#define REFLECTION(name, ...) \
    struct name { \
    __VA_OPT__(MAP(MSTRUCT, __VA_ARGS__)) \
    static ConstDataReflection* _reflection() { \
        const uint32_t reflectionSize = 0 __VA_OPT__(+ NUMARGS(MAP_LIST(MCOUNTER, __VA_ARGS__))); \
        DataTypeSizeMatch types[reflectionSize] = { \
        __VA_OPT__(MAP_LIST(MTYPES, __VA_ARGS__)) \
        }; \
        ConstDataReflection* reflection = new ConstDataReflection(types, reflectionSize); \
        return reflection; \
    } \
    }

#endif // MACROMAGE_H
