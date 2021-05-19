#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <Definitions.h>
#include <EndianConversion.h>
#include <constBin/absConstBinary.h>
#include <constBin/MacroMage.h>
#include <BinaryFileHandler.h>
#include <CRCcalc.h>

#ifndef TESTFILE_PATH
    #define TESTFILE_PATH ""
#endif

namespace BinaryLib {
    class TestClass : public absConstBinary
    {
    public:
        #pragma pack(4)
        struct TestClassData {
            uint32_t rkgd;
            uint16_t fbi_count;
            uint16_t di_count;
            uint16_t ti_count;
            uint16_t padding;

            static ConstDataReflection* _reflection() {
                const uint32_t reflectionSize = 5;

                DataTypeSizeMatch types[reflectionSize] = {
                    DataTypeSizeMatch(DataTypes::UINT32_B),
                    DataTypeSizeMatch(DataTypes::UINT16_B),
                    DataTypeSizeMatch(DataTypes::UINT16_B),
                    DataTypeSizeMatch(DataTypes::UINT16_B),
                    DataTypeSizeMatch(DataTypes::UINT16_B)
                };

                ConstDataReflection* reflection = new ConstDataReflection(types, reflectionSize);

                return reflection;
            }
        } classData;

    public:
        TestClass(uint8_t* data, const uint64_t size, const Endian endian)
            : absConstBinary(TestClassData::_reflection(), endian)
        {
            if(size != reflection->_size() | size != sizeof(TestClassData)) {
                throw std::invalid_argument("size doesn't match data");
            }

            memcpy(&classData, data, size);
        }

        virtual ~TestClass() {}

        BinaryError getDataTable(uint8_t** o_data) override {
            if (o_data == nullptr) {
                return ERROR_NULL_POINTER;
            }

            *o_data = reinterpret_cast<uint8_t*>(&classData);

            return BIN_OK;
        }
    };

    class TestClass2 : public absConstBinary
    {
    public:

        #pragma pack(2)
        struct TestClassData {
            uint64_t rkgd;
            uint8_t arrayTest[20];
            uint16_t padding;

            static ConstDataReflection* _reflection() {
                const uint32_t reflectionSize = 3;

                DataTypeSizeMatch types[reflectionSize] = {
                    DataTypeSizeMatch(DataTypes::UINT64_B),
                    DataTypeSizeMatch(DataTypes::BYTE_ARRAY, 20),
                    DataTypeSizeMatch(DataTypes::UINT16_B)
                };

                ConstDataReflection* reflection = new ConstDataReflection(types, reflectionSize);

                return reflection;
            }
        } classData;

    public:
        TestClass2(uint8_t* data, const uint64_t size, const Endian endian)
            : absConstBinary(TestClassData::_reflection(), endian)
        {
            if(size != reflection->_size() | size != sizeof(TestClassData)) {
                throw std::invalid_argument("size doesn't match data");
            }

            memcpy(&classData, data, size);
        }

        virtual ~TestClass2() {}

        BinaryError getDataTable(uint8_t** o_data) override {
            if (o_data == nullptr) {
                return ERROR_NULL_POINTER;
            }

            *o_data = reinterpret_cast<uint8_t*>(&classData);

            return BIN_OK;
        }
    };

    class TestClass3 : public absConstBinary
    {
    public:
        #pragma pack(2)
        REFLECTION(TestClassData,
                MVAR(MUINT64, rkgd),
                MVAR(MBYTEARRAY, arrayTest, 20),
                MVAR(MUINT16, padding));

        TestClassData classData;

    public:
        TestClass3(uint8_t* data, const uint64_t size, const Endian endian)
            : absConstBinary(TestClassData::_reflection(), endian)
        {
            if(size != reflection->_size() | size != sizeof(TestClassData)) {
                throw std::invalid_argument("size doesn't match data");
            }

            memcpy(&classData, data, size);
        }

        virtual ~TestClass3() {}

        BinaryError getDataTable(uint8_t** o_data) override {
            if (o_data == nullptr) {
                return ERROR_NULL_POINTER;
            }

            *o_data = reinterpret_cast<uint8_t*>(&classData);

            return BIN_OK;
        }
    };



    TEST_CASE("DataTypes Test", "[DataTypes]") {
        cFloat f(0xC4000000U);
        REQUIRE(f._fp() == Approx(-512.0));
        f.u = 0x42FA0000U;
        REQUIRE(f._fp() == Approx(125.0));
        f.f = float(13.5);
        REQUIRE(f._hex() == 0x41580000);
        f.f = float(2.25);
        REQUIRE(f._hex() == 0x40100000);
    }

    TEST_CASE("TestBinaryClass Test", "[Binary]") {
        const uint8_t size = 12;
        uint8_t data[size] = {
            0xA0,
            0x10,
            0x00,
            0x00,
            0x50,
            0x00,
            0x60,
            0x00,
            0x70,
            0x00,
            0x00,
            0x00
        };
        REQUIRE(sizeof(TestClass::TestClassData) == size);

        TestClass x(data, size, Endian::LittleEndian);
        REQUIRE(x.classData.rkgd == 0x000010A0);
        REQUIRE(x.classData.fbi_count == 0x0050);
        REQUIRE(x.classData.di_count == 0x0060);
        REQUIRE(x.classData.ti_count == 0x0070);
        REQUIRE(x.classData.padding == 0x0000);
        REQUIRE(x.getActiveEndian() == Endian::LittleEndian);

        BinaryError err = EndianConversion::convertEndian(&x);
        REQUIRE(err == BinaryError::BIN_OK);
        REQUIRE(x.getActiveEndian() == Endian::BigEndian);
        REQUIRE(x.classData.rkgd == 0xA0100000);
        REQUIRE(x.classData.fbi_count == 0x5000);
        REQUIRE(x.classData.di_count == 0x6000);
        REQUIRE(x.classData.ti_count == 0x7000);
        REQUIRE(x.classData.padding == 0x0000);
    }

    TEST_CASE("TestBinaryClass Test2", "[Binary]") {
        const uint8_t size = 30;
        uint8_t data[size] = {
            0xA0, 0x10, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00,
            0x00,
            0x11,
            0x22,
            0x33,
            0x44,
            0x55,
            0x66,
            0x77,
            0x88,
            0x99,
            0xAA,
            0xBB,
            0xCC,
            0xDD,
            0xEE,
            0xFF,
            0x10,
            0x00,
            0xF1,
            0x00,
            0x00, 0x00
        };
        REQUIRE(sizeof(TestClass2::TestClassData) == size);

        TestClass2 x(data, size, Endian::LittleEndian);
        REQUIRE(x.classData.rkgd == 0x00000050000010A0);
        REQUIRE(x.classData.arrayTest[1] == 0x11);
        REQUIRE(x.classData.arrayTest[9] == 0x99);
        REQUIRE(x.classData.arrayTest[0xF] == 0xFF);
        REQUIRE(x.classData.arrayTest[0x10] == 0x10);
        REQUIRE(x.classData.padding == 0x0000);
        REQUIRE(x.getActiveEndian() == Endian::LittleEndian);

        BinaryError err = EndianConversion::convertEndian(&x);
        REQUIRE(err == BinaryError::BIN_OK);
        REQUIRE(x.getActiveEndian() == Endian::BigEndian);
        REQUIRE(x.classData.rkgd == 0xA010000050000000);
        REQUIRE(x.classData.arrayTest[1] == 0x11);
        REQUIRE(x.classData.arrayTest[9] == 0x99);
        REQUIRE(x.classData.arrayTest[0xF] == 0xFF);
        REQUIRE(x.classData.arrayTest[0x10] == 0x10);
        REQUIRE(x.classData.padding == 0x0000);

        TestClass2 y(data, size, Endian::Exception);
        err = EndianConversion::convertEndian(&y);
        REQUIRE(err == BinaryError::ERROR_INVALID_ENDIAN);
    }

    TEST_CASE("TestBinaryClass Test3", "[Binary]") {
        const uint8_t size = 30;
        uint8_t data[size] = {
            0xA0, 0x10, 0x00, 0x00, 0x50, 0x00, 0x00, 0x00,
            0x00,
            0x11,
            0x22,
            0x33,
            0x44,
            0x55,
            0x66,
            0x77,
            0x88,
            0x99,
            0xAA,
            0xBB,
            0xCC,
            0xDD,
            0xEE,
            0xFF,
            0x10,
            0x00,
            0xF1,
            0x00,
            0x00, 0x00
        };
        REQUIRE(sizeof(TestClass3::TestClassData) == size);

        TestClass3 x(data, size, Endian::LittleEndian);
        REQUIRE(x.classData.rkgd == 0x00000050000010A0);
        REQUIRE(x.classData.arrayTest[1] == 0x11);
        REQUIRE(x.classData.arrayTest[9] == 0x99);
        REQUIRE(x.classData.arrayTest[0xF] == 0xFF);
        REQUIRE(x.classData.arrayTest[0x10] == 0x10);
        REQUIRE(x.classData.padding == 0x0000);
        REQUIRE(x.getActiveEndian() == Endian::LittleEndian);

        BinaryError err = EndianConversion::convertEndian(&x);
        REQUIRE(err == BinaryError::BIN_OK);
        REQUIRE(x.getActiveEndian() == Endian::BigEndian);
        REQUIRE(x.classData.rkgd == 0xA010000050000000);
        REQUIRE(x.classData.arrayTest[1] == 0x11);
        REQUIRE(x.classData.arrayTest[9] == 0x99);
        REQUIRE(x.classData.arrayTest[0xF] == 0xFF);
        REQUIRE(x.classData.arrayTest[0x10] == 0x10);
        REQUIRE(x.classData.padding == 0x0000);
    }

    class TestFileClass : public absConstBinary
    {
    public:
        #pragma pack(4)
        REFLECTION(RksFormat,
                   MVAR(MUINT32, id),
                   MVAR(MUINT32, size),
                   MVAR(MBYTEARRAY, data, 0x44),
                   MVAR(MUINT32, crc32));

        RksFormat rksData;
    public:
        TestFileClass(uint8_t* data, const uint64_t size, const Endian endian)
            : absConstBinary (RksFormat::_reflection(), endian)
        {
            if(size != reflection->_size() | size != sizeof(RksFormat)) {
                throw std::invalid_argument("size doesn't match data");
            }

            memcpy(&rksData, data, size);
        }

        virtual ~TestFileClass() {}

        BinaryError getDataTable(uint8_t** o_data) override {
            if (o_data == nullptr) {
                return ERROR_NULL_POINTER;
            }

            *o_data = reinterpret_cast<uint8_t*>(&rksData);

            return BIN_OK;
        }
    };

    TEST_CASE("FileHandling Test", "[BinaryFile]") {
        uint8_t* data = nullptr;

        std::string filePath(TESTFILE_PATH);
        filePath.append(1, '/');

        std::string outputPath(filePath);
        filePath.append("test.rks");
        outputPath.append("testout.rks");

        const uint64_t size = BinaryFileHandler::readFile(&data, filePath);

        REQUIRE(size == 0x50);

        TestFileClass x(data, size, Endian::BigEndian);

        BinaryError err = EndianConversion::convertEndian(&x);
        REQUIRE(err == BinaryError::BIN_OK);
        REQUIRE(x.getActiveEndian() == Endian::LittleEndian);
        REQUIRE(x.rksData.id == 0x524B534B);
        REQUIRE(x.rksData.size == 0x50);
        REQUIRE(x.rksData.crc32 == 0xB7EE72DE);

        err = EndianConversion::convertEndian(&x);
        REQUIRE(err == BinaryError::BIN_OK);
        REQUIRE(x.getActiveEndian() == Endian::BigEndian);

        x.rksData.data[0] = 0x54;
        uint32_t crc32 = CRCcalc::updateCRC32constOffset(&x, 0x4C);
        REQUIRE(crc32 == 0x554E46A6);
        REQUIRE(x.rksData.crc32 == 0x554E46A6);

        x.rksData.crc32 = 0xFFFFFFFF;
        crc32 = CRCcalc::updateCRC32(&x, 3);
        REQUIRE(crc32 == 0x554E46A6);
        REQUIRE(x.rksData.crc32 == 0x554E46A6);

        err = EndianConversion::convertEndian(&x);
        REQUIRE(err == BinaryError::BIN_OK);
        REQUIRE(x.getActiveEndian() == Endian::LittleEndian);
        REQUIRE(x.rksData.crc32 == 0xA6464E55);

        err = EndianConversion::convertEndian(&x);
        REQUIRE(err == BinaryError::BIN_OK);
        REQUIRE(x.getActiveEndian() == Endian::BigEndian);


        err = BinaryFileHandler::writeFile(&x, outputPath);
        REQUIRE(err == BinaryError::BIN_OK);

        delete[] data;
    }
}

