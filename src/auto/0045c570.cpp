#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, sub_755810, 0x00755810, (unsigned int a1));

// Zeroes the float slot of the object at a1 (a1->+4 points to a block whose +4 is the float).
// Then, if the record referenced by (*a3)->+4 has a non-negative handle field, resolves that
// handle through the current scene. If the resolved record passes the sub_755810 check, the
// integer result of its value object's virtual getter (slot 33, byte offset 132) is stored
// as a float into the same float slot. The function returns the last eax value.
static int __stdcall UpdateHandleRecordFloatValue(int a1, int a2, int a3)
{
    (void)a2;

    // *(*(a1 + 4) + 4) = 0
    int32_t outputPtr = CSF_AT(int32_t, reinterpret_cast<void*>(a1), 4);
    void* outputBlock = reinterpret_cast<void*>(outputPtr);
    CSF_AT(int32_t, outputBlock, 4) = 0;

    // v3 = *(*(a3) + 4); v4 = v3->+12; result = v3 + 4
    int32_t entryPtr = CSF_AT(int32_t, reinterpret_cast<void*>(a3), 0);
    int32_t recordPtr = CSF_AT(int32_t, reinterpret_cast<void*>(entryPtr), 4);
    int32_t handleField = CSF_AT(int32_t, reinterpret_cast<void*>(recordPtr), 12);

    int result = recordPtr + 4;
    if (handleField >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        int32_t resolvedRecord = result;
        if (resolvedRecord != 0)
        {
            uint32_t recordFlag = CSF_AT(uint32_t, reinterpret_cast<void*>(resolvedRecord), 80);
            result = sub_755810(recordFlag);
            if (static_cast<uint8_t>(result) != 0)
            {
                // v7 = resolved->+56; the output block pointer is re-read after the calls.
                int32_t valueObject = CSF_AT(int32_t, reinterpret_cast<void*>(resolvedRecord), 56);
                void** valueVtbl = CSF_AT(void**, reinterpret_cast<void*>(valueObject), 0);
                auto getValue = reinterpret_cast<int(__fastcall*)(void*, void*)>(valueVtbl[132 / 4]);
                result = getValue(reinterpret_cast<void*>(valueObject), nullptr);

                // v8 = a1->+4 (re-read); *(float*)(v8 + 4) = (float)result
                int32_t currentOutputPtr = CSF_AT(int32_t, reinterpret_cast<void*>(a1), 4);
                CSF_AT(float, reinterpret_cast<void*>(currentOutputPtr), 4) = static_cast<float>(result);
            }
        }
    }
    return result;
}

HOOK(0x0045C570, UpdateHandleRecordFloatValue, "auto");
