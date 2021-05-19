#ifndef BINARYMEMBER_H
#define BINARYMEMBER_H

#include "IBinary.h"

namespace BinaryLib {
    struct BinaryMember {
    protected:
        BinaryMember(){}
    };

    template<typename T>
    struct BinaryRef : public BinaryMember
    {
        T var;

        explicit BinaryRef(const T var)
            : var(var)
        {}

        operator T() const {
            return var;
        }
    };

    template<typename T, typename S>
    struct BinaryListAbsolute : public BinaryMember
    {
    private:
        T* list;
        uint32_t listSize;
        uint32_t allocSize;

        DTSM_List<S>* reflectionRef;


    public:
        BinaryListAbsolute()
            : list(nullptr),
              listSize(0),
              allocSize(0),
              reflectionRef(nullptr)
        {}
        virtual ~BinaryListAbsolute() {
            if (list != nullptr) {
                delete [] list;
                list = nullptr;
            }
        }

        BinaryError init(DTSM_List<S>* reflectionRef, const T* const data, const uint32_t listSize) {
            if (reflectionRef == nullptr | data == nullptr) {
                return ERROR_NULL_POINTER;
            }

            if (reflectionRef->_dtype() < DTSM_Type::DTSMLIST) {
                return BinaryError::ERROR_INVALID_REFLECTION;
            }

            this->reflectionRef = reflectionRef;
            this->listSize = listSize;

            allocSize = (listSize / 10 + 1) * 10;

            list = new T[allocSize];

            memcpy(list, data, listSize * sizeof(T));

            return update();
        }

        BinaryError init(DTSM_List<S>* reflectionRef, const uint8_t* const data, const uint64_t size, const uint32_t listSize) {
            if (reflectionRef == nullptr | data == nullptr) {
                return ERROR_NULL_POINTER;
            }

            if (reflectionRef->_dtype() < DTSM_Type::DTSMLIST) {
                return BinaryError::ERROR_INVALID_REFLECTION;
            }

            this->reflectionRef = reflectionRef;
            this->listSize = listSize;
            allocSize = listSize;

            uint8_t* copyData = new uint8_t[size];
            list = reinterpret_cast<T*>(copyData);

            memcpy(list, data, size);

            return update();
        }

    private:
        BinaryError update() {
            if(reflectionRef == nullptr) {
                return ERROR_NULL_POINTER;
            }

            reflectionRef->indexSize = listSize;

            return BIN_OK;
        }

    public:
        BinaryError add(const T& element, const uint32_t index = 0xFFFFFFFF) {
            if(listSize > allocSize) {
                return ERROR_OUT_OF_BOUNDS;
            }

            if(listSize == allocSize) {
                allocSize += 10;
                T* newAlloc = new T[allocSize]();
                memcpy(newAlloc, list, listSize * sizeof(T));

                delete [] list;
                list = newAlloc;
            }

            if(index > listSize & index != 0xFFFFFFFF) {
                return ERROR_OUT_OF_BOUNDS;
            }
            else if(index == 0xFFFFFFFF | index == listSize) {
                memcpy(list + listSize, &element, sizeof(T));
            }
            else {
                T* newAlloc = new T[listSize - index];

                memcpy(newAlloc, list + index, (listSize - index) * sizeof(T));
                memcpy(list + index, &element, sizeof(T));
                memcpy(list + index + 1, newAlloc, (listSize - index) * sizeof(T));

                delete [] newAlloc;
            }

            listSize++;

            return update();
        }

        BinaryError del(const uint32_t index = 0xFFFFFFFF) {
            if(index > listSize & index != 0xFFFFFFFF) {
                return ERROR_OUT_OF_BOUNDS;
            }

            if(listSize <= (allocSize - 15)) {
                allocSize -= 10;
                T* newAlloc = new T[allocSize]();
                memcpy(newAlloc, list, listSize * sizeof(T));

                delete [] list;
                list = newAlloc;
            }

            if(index != 0xFFFFFFFF | index != (listSize - 1)) {
                const uint32_t pIndex = index + 1;

                T* newAlloc = new T[listSize - pIndex];

                memcpy(newAlloc, list + pIndex, (listSize - pIndex) * sizeof(T));
                memcpy(list + index, newAlloc, (listSize - pIndex) * sizeof(T));

                delete [] newAlloc;
            }

            listSize--;

            return update();
        }

        uint32_t _data(uint8_t** o_data) {
            *o_data = list;

            return listSize * sizeof(T);
        }

        BinaryError update(const T& element, const uint32_t index) {
            if(index >= listSize) {
                return ERROR_OUT_OF_BOUNDS;
            }

            memcpy(list + index, &element, sizeof(T));

            return BIN_OK;
        }

        T getElement(const uint32_t index) const {
            T element;
            if (index >= listSize) {
                return element;
            }

            memcpy(&element, list + index, sizeof(T));

            return element;
        }

        T operator[] (const uint32_t index) const {
            return getElement(index);
        }
    };

    template<typename T>
    struct BinaryList;

    template <> struct BinaryList<uint8_t> : public BinaryListAbsolute<uint8_t, DTSM_Ref> {};
    template <> struct BinaryList<uint16_t> : public BinaryListAbsolute<uint16_t, DTSM_Ref> {};
    template <> struct BinaryList<uint32_t> : public BinaryListAbsolute<uint32_t, DTSM_Ref> {};
    template <> struct BinaryList<uint64_t> : public BinaryListAbsolute<uint64_t, DTSM_Ref> {};
    template <> struct BinaryList<int8_t> : public BinaryListAbsolute<int8_t, DTSM_Ref> {};
    template <> struct BinaryList<int16_t> : public BinaryListAbsolute<int16_t, DTSM_Ref> {};
    template <> struct BinaryList<int32_t> : public BinaryListAbsolute<int32_t, DTSM_Ref> {};
    template <> struct BinaryList<int64_t> : public BinaryListAbsolute<int64_t, DTSM_Ref> {};
    template <> struct BinaryList<float> : public BinaryListAbsolute<float, DTSM_Ref> {};
    template <> struct BinaryList<double> : public BinaryListAbsolute<double, DTSM_Ref> {};
    template <typename T> struct BinaryList : public BinaryListAbsolute<T, DTSM_Struct> {};
}

#endif // BINARYMEMBER_H
