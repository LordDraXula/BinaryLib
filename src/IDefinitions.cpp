#include "IDefinitions.h"

namespace BinaryLib {
    IBinLink::IBinLink() {}
    IBinLink::~IBinLink() {}
    uint64_t IBinLink::_size() const {return 0;}
    uint64_t IBinLink::_offset(const uint32_t index) const {return 0;}

    IDTSM::IDTSM() {}
    IDTSM::~IDTSM() {}
    DTSM_Type IDTSM::_dtype() const {return DTSM_Type::DTSMREF;}
    uint64_t IDTSM::find(const IDTSM* const ref) const {return 0;}

    IReflection::IReflection() {}
    IReflection::~IReflection() {}
}
