#ifndef ABSCONSTBINARY_H
#define ABSCONSTBINARY_H

#include "../absBinary.h"
#include "../Reflection.h"

namespace BinaryLib {
    class absConstBinary : public absBinary
    {
    public:
        absConstBinary(ConstDataReflection* reflection, const Endian endian);
    };
}


#endif // ABSCONSTBINARY_H
