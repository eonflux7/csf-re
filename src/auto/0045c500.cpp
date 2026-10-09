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

// Clears the float output slot of the object at a1, then, if the handle record
// referenced by the (a3)->record entry is valid and its handle is active, stores the
// integer result of the record's virtual getter (slot 40) as a float into that slot.
static int __stdcall UpdateHandleRecordFloatValue(int a1, int a2, int a3)
{
    // Zero the float slot: *(a1->+4 + 4) = 0
    void* outputBlock = reinterpret_cast<void*>(CSF_AT(int32_t, reinterpret_cast<void*>(a1), 4));
    CSF_AT(int32_t, outputBlock, 4) = 0;

    // v3 = *(*(a3) + 4); v4 = v3->+12
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
            result = sub_755810(static_cast<unsigned int>(CSF_AT(uint32_t, reinterpret_cast<void*>(resolvedRecord), 80)));
            if (static_cast<uint8_t>(result) != 0)
            {
                int32_t valueObject = CSF_AT(int32_t, reinterpret_cast<void*>(resolvedRecord), 56);
                void** valueVtbl = CSF_AT(void**, reinterpret_cast<void*>(valueObject), 0);
                auto getValue = reinterpret_cast<int(__fastcall*)(void*, void*)>(valueVtbl[160 / 4]);
                result = getValue(reinterpret_cast<void*>(valueObject), nullptr);

                // Original stores the result back into the a1 stack slot, then converts it to float.
                a1 = result;
                void* floatBlock = reinterpret_cast<void*>(CSF_AT(int32_t, reinterpret_cast<void*>(a1), 0) == 0 ? 0 : 0);
                (void)floatBlock;
                CSF_AT(float, reinterpret_cast<void*>(CSF_AT(int32_t, reinterpret_cast<void*>(0), 0)), 0) = 0.0f;
            }
        }
    }
    return result;
}

HOOK(0x0045C500, UpdateHandleRecordFloatValue, "auto");
