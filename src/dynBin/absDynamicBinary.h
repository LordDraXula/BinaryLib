#ifndef ABSDYNAMICBINARY_H
#define ABSDYNAMICBINARY_H

#include "../absBinary.h"
#include "../Reflection.h"
#include "../BinaryMember.h"

namespace BinaryLib {
#pragma pack(push, 4)
    class absDynamicBinary : public absBinary
    {
    protected:

    private:
        BinaryError handleInitSection(IDTSM* currentDTSM, const uint8_t** data, const uint8_t* const dataCopy, uint8_t** structData);

    protected:
        BinaryError init(const uint8_t* data);

    public:
        absDynamicBinary(DynDataReflection* reflection, const Endian endian);

        BinaryError getFullDataTable(uint8_t** o_data);
    };
#pragma pack(pop)
}


#endif // ABSDYNAMICBINARY_H
