#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch.hpp>

#include <Definitions.h>
#include <BinaryMember.h>
#include <Reflection.h>
#include <dynBin/absDynamicBinary.h>
#include <EndianConversion.h>
using namespace BinaryLib;



class TestClass : public absDynamicBinary
{
public:
    #pragma pack(4)
    struct DataStruct
    {
        uint16_t col;
        uint8_t i1;
        uint8_t i2;
    };

    #pragma pack(4)
    struct TestClassData
    {
        uint32_t id;
        uint32_t size;
        BinaryList<DataStruct> data;

        static DynDataReflection* _reflection() {
            const uint8_t dataTestSSize = 3;
            IDTSM** dataTestSReflection = new IDTSM*[dataTestSSize]
            {
                new DTSM_Ref(DataTypes::UINT16_B),
                new DTSM_Ref(DataTypes::UINT8_B),
                new DTSM_Ref(DataTypes::UINT8_B)
            };

            const uint8_t listSize = 3;
            IDTSM** list = new IDTSM*[listSize]
            {
                new DTSM_Ref(DataTypes::UINT32_B),
                DTSM_Factory::_ref("size", DataTypes::UINT32_B),
                new STRUCT_List(new DTSM_Struct(dataTestSReflection, dataTestSSize), DTSM_Factory::_ref("size"))
            };

            DynDataReflection* reflection = new DynDataReflection(list, listSize);

            return reflection;
        }
    } classData;

public:
    TestClass(uint8_t* data, const uint64_t size, const Endian endian)
        : absDynamicBinary(TestClassData::_reflection(), endian)
    {
        init(data, size);
    }

    virtual ~TestClass() override {}

    BinaryError getDataTable(uint8_t** o_data) override {
        if (o_data == nullptr) {
            return BinaryError::ERROR_NULL_POINTER;
        }

        *o_data = reinterpret_cast<uint8_t*>(&classData);

        return BinaryError::BIN_OK;
    }
};

TEST_CASE("Binary Size Test", "[BinarySize]") {
    buint8 x(0);

    REQUIRE(sizeof(x) == sizeof(uint8_t));
    REQUIRE(sizeof(BinaryRef<uint32_t>) == sizeof(uint32_t));
    REQUIRE(sizeof(BinaryList<uint8_t>) == sizeof(BinaryList<uint64_t>));
    REQUIRE(sizeof(BinaryList<uint8_t>) == sizeof(BinaryList<TestClass::DataStruct>));

    uint8_t test(200);

    x = static_cast<buint8>(test);

    REQUIRE(x.var == 200);

    uint16_t arr[] = {
        5,4,8
    };

    BinaryMember* list = reinterpret_cast<BinaryMember*>(arr);

    buint16 test2 = *static_cast<buint16*>(&list[0]);
    buint32 test3 = *static_cast<buint32*>(&list[0]);

    REQUIRE(test2 == 5);
    REQUIRE(test3 == 0x00040005);

    cUtilUn syz(256);
    REQUIRE(syz.u8 == 0);
    REQUIRE(syz.u16 == 256);
    REQUIRE(syz.u32 == 256);


    const uint8_t listSize = 2;
    const uint8_t dataSize = listSize*sizeof(TestClass::DataStruct);
    REQUIRE(dataSize == 8);

    uint8_t* data = new uint8_t[dataSize + 8] {
                    0x00, 0x00, 0x40, 0x10,
                    0x00, 0x00, 0x00, listSize,
                    0x50,
                    0x60,
                    0x1F,
                    0x20,
                    0x70,
                    0x60,
                    0x42,
                    0x43};

    cUtilUn syz2(data + 8, 2);
    REQUIRE(syz2.u8 == 0x50);
    REQUIRE(syz2.u16 == 0x6050);
    REQUIRE(syz2.u64 == 0x6050);

    TestClass testClass(data, dataSize + 8, Endian::BigEndian);

    DynDataReflection* reflection = static_cast<DynDataReflection*>(testClass.getDataReflection());

    REQUIRE(reflection->_size() == 16);
    REQUIRE(reflection->_type() == BinaryType::BinDyn);
    REQUIRE(static_cast<STRUCT_List*>(reflection->dtsmList[2])->_dtype() == DTSM_Type::DTSMSTRUCTLIST);

    REQUIRE(testClass.classData.data.getElement(1).col == 0x6070);
    REQUIRE(testClass.classData.data.getElement(0).i1 == 0x1F);

    REQUIRE(testClass.getFullDataTable(&data) == BinaryError::BIN_OK);
    REQUIRE(data[13] == 0x60);

    REQUIRE(EndianConversion::convertEndian(&testClass) == BinaryError::BIN_OK);
    REQUIRE(testClass.getFullDataTable(&data) == BinaryError::BIN_OK);
    REQUIRE(testClass.classData.id == 0x00004010);
    REQUIRE(testClass.classData.data[0].col == 0x5060);
    REQUIRE(testClass.classData.data[1].col == 0x7060);
    REQUIRE(data[13] == 0x70);
    REQUIRE(data[1] == 0x40);
}
