#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* edx, int additionalCapacity));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

typedef unsigned char (__fastcall *ArchiveCountFn)(void* self, void* edx, uint32_t* count, int archive);
typedef unsigned char (__fastcall *ArchiveEndFn)(void* self, void* edx, int archive);

static unsigned int __stdcall Scene_SerializeNameList(S_0041e5f0* a1, int a2)
{
    uint8_t* ar = reinterpret_cast<uint8_t*>(a2);
    uint32_t count = 0;
    unsigned int result;
    uint8_t flag = 0;
    uint8_t keepFlag = CSF_AT(uint8_t, ar, 0x15);
    void* stream;

    if (CSF_AT(uint8_t, ar, 0x14))
    {
        count = a1->field_0008;
        stream = CSF_AT(void*, ar, 4);
        if (ar[0] == 0 &&
            ((ArchiveCountFn)CSF_AT(void**, stream, 0)[30])(stream, nullptr, &count, a2) == 0)
            result = 1;
        else
            result = 0;
        ar[0] = (uint8_t)result;
        if ((uint8_t)result)
            goto fail;
        for (uint32_t i = 0; i < count; ++i)
        {
            void* elem = reinterpret_cast<void*>(a1->field_0000 + 4 * i);
            bool wasErr = ar[0] == 0;
            CSF_AT(uint32_t, ar, 0x1C) = 0;
            if (wasErr)
            {
                CSF_AT(uint8_t, ar, 0x15) = keepFlag;
                CSF_AT(uint8_t, ar, 0x16) = 0;
                Archive_SerializeObjectRef(elem, reinterpret_cast<bool*>(ar));
            }
        }
        if (ar[0])
            goto fail;
    }
    else
    {
        stream = CSF_AT(void*, ar, 4);
        if (ar[0] == 0 &&
            ((ArchiveCountFn)CSF_AT(void**, stream, 0)[30])(stream, nullptr, &count, a2) == 0)
            result = 1;
        else
            result = 0;
        ar[0] = (uint8_t)result;
        if ((uint8_t)result)
            goto fail;
        if (keepFlag)
        {
            for (uint32_t j = 0; j < count; ++j)
            {
                bool wasErr = ar[0] == 0;
                void* elem = reinterpret_cast<void*>(a1->field_0000 + 4 * j);
                CSF_AT(uint32_t, ar, 0x1C) = 0;
                if (wasErr)
                {
                    CSF_AT(uint8_t, ar, 0x15) = keepFlag;
                    CSF_AT(uint8_t, ar, 0x16) = 0;
                    Archive_SerializeObjectRef(elem, reinterpret_cast<bool*>(ar));
                }
            }
        }
        else
        {
            uint32_t cap = a1->field_0004;
            a1->field_0008 = 0;
            if (count > cap)
                S_0041e5f0_GrowArrayCapacity(a1, nullptr, (int)(count - cap));
            uint32_t idx = 0;
            if (count)
            {
                do
                {
                    if (a1->field_0008 >= a1->field_0004)
                        S_0041e5f0_GrowArrayCapacity(a1, nullptr, (int)a1->field_000c);
                    uint32_t n = a1->field_0008;
                    uint32_t* slot = reinterpret_cast<uint32_t*>(a1->field_0000 + 4 * n);
                    a1->field_0008 = n + 1;
                    *slot = 0;
                    bool wasErr = ar[0] == 0;
                    void* elem = reinterpret_cast<void*>(a1->field_0000 + 4 * idx);
                    CSF_AT(uint32_t, ar, 0x1C) = 0;
                    if (wasErr)
                    {
                        CSF_AT(uint8_t, ar, 0x15) = 0;
                        CSF_AT(uint8_t, ar, 0x16) = 0;
                        Archive_SerializeObjectRef(elem, reinterpret_cast<bool*>(ar));
                    }
                    ++idx;
                } while (idx < count);
            }
        }
        if (ar[0])
            goto fail;
    }

    stream = CSF_AT(void*, ar, 4);
    result = ((ArchiveEndFn)CSF_AT(void**, stream, 0)[31])(stream, nullptr, a2);
    if (!(uint8_t)result)
        goto fail;
    ar[0] = flag;
    return result;

fail:
    flag = 1;
    ar[0] = flag;
    return result;
}

HOOK(0x004E8060, Scene_SerializeNameList, "auto");
