#include "absBinary.h"

namespace BinaryLib {
    absBinary::absBinary(IReflection* reflection, const Endian endian)
        : endian(endian)
    {
        this->reflection = reflection;
    }

    absBinary::~absBinary() {
        delete reflection;
        reflection = nullptr;
    }

    IReflection* absBinary::getDataReflection() {
        return reflection;
    }

    Endian absBinary::getActiveEndian() const {
        return endian;
    }

    void absBinary::setActiveEndian(const Endian endian) {
        this->endian = endian;
    }
}

