#ifndef ABSBINARY_H
#define ABSBINARY_H

#include "IBinary.h"

namespace BinaryLib {
    class absBinary : public IBinary
    {
    protected:
        IReflection* reflection;
        Endian endian;

    public:
        absBinary(IReflection* reflection, const Endian endian);
        virtual ~absBinary() override;

        IReflection* getDataReflection() override;

        Endian getActiveEndian() const override;
        void setActiveEndian(const Endian endian) override;
    };
}


#endif // ABSBINARY_H
