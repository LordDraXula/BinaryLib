#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <cstdint>

#include "DTSM.h"

#define RETURN_ON_ERROR(ERR) if(ERR != BinaryError::BIN_OK) return ERR

namespace BinaryLib {
    /**
     * @brief An union representing a floating point value
     * @details Allows easy conversion between hex and floating point handling
     */
    template<typename S, typename T>
    union cHexFP {
        T u;
        S f;

        cHexFP() {
            u = 0;
        }

        cHexFP(S f) : f(f)
        {
        }

        cHexFP(T u) : u(u)
        {
        }

        const T _hex() const {
            return u;
        }

        const S _fp() const {
            return f;
        }

        bool operator==(const cHexFP &rhs) const {
            return u == rhs.u;
        }

        bool operator!=(const cHexFP &rhs) const {
            return !operator==(rhs);
        }

        operator T() const {
            return u;
        }

        operator S() const {
            return f;
        }
    };

    typedef cHexFP<double, uint64_t> cDouble;
    typedef cHexFP<float, uint32_t> cSingle;
    typedef cSingle cFloat;

    union cUtilUn {
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;
        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;
        float f;
        double d;

        cUtilUn(const uint8_t u8) : u8(u8) {}
        cUtilUn(const uint16_t u16) : u16(u16) {}
        cUtilUn(const uint32_t u32) : u32(u32) {}
        cUtilUn(const uint64_t u64) : u64(u64) {}
        cUtilUn(const int8_t i8) : i8(i8) {}
        cUtilUn(const int16_t i16) : i16(i16) {}
        cUtilUn(const int32_t i32) : i32(i32) {}
        cUtilUn(const int64_t i64) : i64(i64) {}
        cUtilUn(const float f) : f(f) {}
        cUtilUn(const double d) : d(d) {}

        cUtilUn(const uint8_t* const d, const uint8_t s) {
            u64 = 0;

            memcpy(&u64, d, s);
        }
    };

    template<typename T>
    struct BinaryRef;

    typedef BinaryRef<uint8_t> buint8;
    typedef BinaryRef<uint16_t> buint16;
    typedef BinaryRef<uint32_t> buint32;
    typedef BinaryRef<uint64_t> buint64;
    typedef BinaryRef<int8_t> bint8;
    typedef BinaryRef<int16_t> bint16;
    typedef BinaryRef<int32_t> bint32;
    typedef BinaryRef<int64_t> bint64;
    typedef BinaryRef<float> bfloat;
    typedef BinaryRef<double> bdouble;
    typedef bfloat bsingle;
}


#endif // DEFINITIONS_H
