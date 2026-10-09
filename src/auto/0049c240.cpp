#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Element (8 bytes) serializer: ecx = element, arg = stream
CSF_FUNC(int, __fastcall, sub_44EBF0, 0x0044EBF0, (void* self, void* edx, int a2));
// Vector reserve/resize to the given element count
CSF_FUNC(unsigned int, __fastcall, sub_49B1D0, 0x0049B1D0, (void* self, void* edx, unsigned int a2));
// Vector push_back of an 8-byte element
CSF_FUNC(unsigned int, __fastcall, sub_49C1D0, 0x0049C1D0, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790, (int* self, void* edx));

// Serializes one element of the array through the stream, restoring the per-element stream flags.
static inline void SerializeArrayElement(S_0047ea40* vec, unsigned int index, void* stream, uint8_t flag15)
{
    void* element = reinterpret_cast<uint8_t*>(vec->field_0004) + index * 8;
    bool streamFailed = CSF_AT(uint8_t, stream, 0x0) != 0;
    CSF_AT(uint32_t, stream, 0x1C) = 0;
    if (!streamFailed)
    {
        CSF_AT(uint8_t, stream, 0x15) = flag15;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        sub_44EBF0(element, nullptr, reinterpret_cast<int>(stream));
    }
}

static void __stdcall Stream_SerializePairArray(S_0047ea40* vec, void* stream)
{
    const uint8_t savedFlag15 = CSF_AT(uint8_t, stream, 0x15);
    unsigned int count = 0;
    bool failed;

    if (CSF_AT(uint8_t, stream, 0x14) != 0)
    {
        // Writing: count comes from the container
        if (vec->field_0004 != 0)
            count = static_cast<unsigned int>(static_cast<int>(vec->field_0008 - vec->field_0004) >> 3);
        else
            count = 0;

        failed = false;
        if (CSF_AT(uint8_t, stream, 0x0) == 0)
        {
            void* impl = CSF_AT(void*, stream, 0x4);
            if (!((bool(__fastcall*)(void*, void*, unsigned int*, void*))CSF_AT(void**, impl, 0)[0x78 / 4])(impl, nullptr, &count, stream))
                failed = true;
        }
        CSF_AT(uint8_t, stream, 0x0) = failed ? 1 : 0;

        if (!failed)
        {
            for (unsigned int i = 0; i < count; ++i)
                SerializeArrayElement(vec, i, stream, savedFlag15);
        }
    }
    else
    {
        // Reading: count comes from the stream
        failed = false;
        if (CSF_AT(uint8_t, stream, 0x0) == 0)
        {
            void* impl = CSF_AT(void*, stream, 0x4);
            if (!((bool(__fastcall*)(void*, void*, unsigned int*, void*))CSF_AT(void**, impl, 0)[0x78 / 4])(impl, nullptr, &count, stream))
                failed = true;
        }
        CSF_AT(uint8_t, stream, 0x0) = failed ? 1 : 0;

        if (!failed)
        {
            if (savedFlag15 != 0)
            {
                for (unsigned int i = 0; i < count; ++i)
                    SerializeArrayElement(vec, i, stream, savedFlag15);
            }
            else
            {
                DestroyPointerStorage(reinterpret_cast<int*>(vec), nullptr);
                sub_49B1D0(vec, nullptr, count);

                if (count > 0)
                {
                    uint32_t defaultElement[2];
                    defaultElement[0] = 1;
                    defaultElement[1] = 0xFFFFFFFF;

                    unsigned int index = 0;
                    do
                    {
                        sub_49C1D0(vec, nullptr, defaultElement);
                        SerializeArrayElement(vec, index, stream, 0);
                        ++index;
                    } while (index < count);
                }
            }
        }
    }

    uint8_t finalError = 0;
    if (failed)
    {
        finalError = 1;
    }
    else if (CSF_AT(uint8_t, stream, 0x0) != 0)
    {
        finalError = 1;
    }
    else
    {
        void* impl = CSF_AT(void*, stream, 0x4);
        if (!((bool(__fastcall*)(void*, void*, void*))CSF_AT(void**, impl, 0)[0x7C / 4])(impl, nullptr, stream))
            finalError = 1;
    }
    CSF_AT(uint8_t, stream, 0x0) = finalError;
}

HOOK(0x0049C240, Stream_SerializePairArray, "auto");
