#ifndef EXAMPLECLASS_H
#define EXAMPLECLASS_H

#include "absBinary.h"
using namespace BinaryLib;

#include "MacroMage.h"

/**
 * @brief Example Class demonstrating usage
 */
class ExampleClass : public absBinary
{
private:
    #pragma pack(4)
    struct ExampleStruct {
        uint32_t id;
        uint32_t size;
        uint16_t setting;
        uint16_t padding;
        uint8_t data[12];

        static DataReflection* _reflection() {
            const uint32_t reflectionSize = 5;

            DataTypeSizeMatch types[reflectionSize] = {
                DataTypeSizeMatch(DataTypes::UINT32_B),
                DataTypeSizeMatch(DataTypes::UINT32_B),
                DataTypeSizeMatch(DataTypes::UINT16_B),
                DataTypeSizeMatch(DataTypes::UINT16_B),
                DataTypeSizeMatch(DataTypes::BYTE_ARRAY, 12),
            };

            DataReflection* reflection = new DataReflection(types, reflectionSize);

            return reflection;
        }

    } exampleStruct;

public:
    ExampleClass(uint8_t* data, const uint64_t size, const Endian endian);

    uint8_t* getStructData() override;
};

/**
 * @brief Same Example Class as @ref ExampleClass, but using @headerfile MacroMage.h
 */
class ExampleClass2 : public absBinary
{
private:
    #pragma pack(4)
    REFLECTION(ExampleStruct,
               MVAR(MUINT32, id),
               MVAR(MUINT32, size),
               MVAR(MUINT16, setting),
               MVAR(MUINT16, padding),
               MVAR(MBYTEARRAY, data, 12));

    ExampleStruct exampleStruct;

public:
    ExampleClass2(uint8_t* data, const uint64_t size, const Endian endian);

    uint8_t* getStructData() override;
};

#endif // EXAMPLECLASS_H
