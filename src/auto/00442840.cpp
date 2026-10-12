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

CSF_FUNC(int*, __fastcall, MissionScript_SetValueSlot, 0x00469670,
         (S_004427c0* self, void* edx, int* outputSlot, int currentValue, int previousValue));
CSF_FUNC(int, __fastcall, DynamicObj_GrowEntryVector, 0x004424F0,
         (void* self, void* edx, int a1, unsigned int a3, uint32_t* arg8));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Resizes the 16-byte-element entry array of a dynamic object to a2 elements.
// Shrinks by calling the value-slot helper from index a2; grows by calling the
// grow helper with a scratch slot (a3) that is freed afterwards if it was filled.
static int __fastcall DynamicObj_ResizeEntryVector(S_004427c0* self, void* /*edx*/,
                                                   unsigned int a2, int a3, int a4, int a5, int a6)
{
    uint32_t begin = self->field_0004;

    uint32_t elementCount = 0;
    if (begin) {
        elementCount = static_cast<uint32_t>(static_cast<int>(self->field_0008 - begin) >> 4);
    }

    if (elementCount >= a2) {
        if (begin && a2 < elementCount) {
            MissionScript_SetValueSlot(self, nullptr,
                                       reinterpret_cast<int*>(&a2),
                                       static_cast<int>(begin + 16u * a2),
                                       static_cast<int>(self->field_0008));
        }
    } else {
        DynamicObj_GrowEntryVector(self, nullptr,
                                   static_cast<int>(self->field_0008),
                                   a2 - elementCount,
                                   reinterpret_cast<uint32_t*>(&a3));
    }

    int result = a3;
    if (a3) {
        return Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(a3)));
    }
    return result;
}

HOOK(0x00442840, DynamicObj_ResizeEntryVector, "auto");
