#ifndef DTSM_H
#define DTSM_H

#include <cstring>
#include <type_traits>

#include <map>

#include "IDefinitions.h"

#define DTSM_LIST_MSG "Type must be DTSM_Ref or DTSM_Struct"

namespace BinaryLib {
#pragma pack(push, 4)
    struct DataTypeSizeMatch {
        DataTypes t;
        uint32_t s;

        DataTypeSizeMatch(const DataTypes t = UINT8_B, const uint32_t dataSize = 0);
    };

    struct BinStruct : public IBinLink
    {
        DataTypeSizeMatch* types;
        const uint32_t indexSize;

        BinStruct(const DataTypeSizeMatch* types, const uint32_t indexSize);
        BinStruct(const BinStruct& other);

        virtual ~BinStruct() override;

        uint64_t _size() const override;
        uint64_t _offset(const uint32_t index) const override;
    };

    struct DTSM_Ref : public IDTSM, DataTypeSizeMatch
    {
        DTSM_Ref(const DataTypes t = UINT8_B, const uint32_t dataSize = 0);
        virtual ~DTSM_Ref() override;

        uint64_t _size() const override;
        uint64_t _offset(const uint32_t index) const override;
        DTSM_Type _dtype() const override;

        uint64_t find(const IDTSM* const ref) const override;
    };

    struct DTSM_Struct : public IDTSM
    {
        IDTSM** types;
        uint32_t indexSize;

        DTSM_Struct(IDTSM** types, const uint32_t indexSize);
        virtual ~DTSM_Struct() override;

        uint64_t _size() const override;
        uint64_t _offset(const uint32_t index) const override;
        DTSM_Type _dtype() const override;

        uint64_t find(const IDTSM* const ref) const override;
    };


    template<typename T>
    struct DTSM_List : public IDTSM
    {
        static_assert (std::is_base_of<IDTSM, T>::value, DTSM_LIST_MSG);
        static_assert (!std::is_same<DTSM_List, T>::value, DTSM_LIST_MSG);

        T* type;
        uint32_t indexSize;

        DTSM_Ref* indexRef;

        DTSM_List(T* type, DTSM_Ref* indexRef)
            : indexSize(0)
        {
            this->type = type;
            this->indexRef = indexRef;
        }

        virtual ~DTSM_List() {
            delete type;
            type = nullptr;
        }

        uint64_t _size() const override {
            return _offset(indexSize);
        }
        uint64_t _offset(const uint32_t index) const override {
            return type->_size() * index;
        }
        DTSM_Type _dtype() const override {
            return static_cast<DTSM_Type>(DTSM_Type::DTSMLIST + type->_dtype());
        }

        uint64_t find(const IDTSM* const ref) const override {
            if(ref == this) {
                return 0;
            }

            return type->find(ref);
        }
    };

    class DTSM_Factory
    {
    private:
        DTSM_Factory();
        virtual ~DTSM_Factory();

        typedef std::map<std::string, DTSM_Ref*> DTSM_Map;
        static DTSM_Map storage;
    public:
        static DTSM_Ref* _ref(const std::string key, const DataTypes dataType = UINT8_B);
    };

    typedef DTSM_List<DTSM_Ref> REF_List;
    typedef DTSM_List<DTSM_Struct> STRUCT_List;

#pragma pack(pop)
}

#endif // DTSM_H
