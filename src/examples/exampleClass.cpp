#include "exampleClass.h"
#include <stdexcept>

ExampleClass::ExampleClass(uint8_t* data, const uint64_t size, const Endian endian)
    : BinaryLib::absBinary(ExampleStruct::_reflection(), endian)
{
    if(size != reflection->_size() | size != sizeof(ExampleStruct)) {
        throw std::invalid_argument("size doesn't match data");
    }

    memcpy(&exampleStruct, data, size);
}

uint8_t* ExampleClass::getStructData() {
    return (uint8_t*) &exampleStruct;
}

ExampleClass2::ExampleClass2(uint8_t* data, const uint64_t size, const Endian endian)
    : BinaryLib::absBinary(ExampleStruct::_reflection(), endian)
{
    if(size != reflection->_size() | size != sizeof(ExampleStruct)) {
        throw std::invalid_argument("size doesn't match data");
    }

    memcpy(&exampleStruct, data, size);
}

uint8_t* ExampleClass2::getStructData() {
    return (uint8_t*) &exampleStruct;
}
