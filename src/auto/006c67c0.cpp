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

// Callees of the original function, bound by address.
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_6C63D0, 0x006C63D0, (void* self, void* edx, int a2));

// Original: char __thiscall sub_6C67C0(C_008656d0* this, C_0086754c* a2), ret 4
static char __fastcall C_008656d0_ProcessSkinnedItemCount(C_008656d0* self, void* /*edx*/, C_0086754c* a2)
{
    // Bump the visit counter stored at +0xFC.
    int visitCount = CSF_AT(int, self, 0xFC);
    CSF_AT(int, self, 0xFC) = visitCount + 1;

    uint8_t flags = static_cast<uint8_t>(self->field_0008);
    char processed = 0;

    if ((flags & 8) == 0)
        return processed;

    int itemCount = GetCurrentItemCount(a2, nullptr);
    processed = sub_6C63D0(self, nullptr, itemCount);

    if (processed)
    {
        CSF_AT(int, self, 0x10C) += 1;

        // a2->vGetValue_10(): vtable slot 0x10 / 4 = 4, called with a2 as this.
        int valueResult = reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, a2, 0)[4])(a2, nullptr);

        if (valueResult == 0)
        {
            // (*(a2->field_0020 + 0x24))(&a2->field_0020): vtable slot 0x24 / 4 = 9.
            uint32_t subVtblPtr = CSF_AT(uint32_t, a2, 0x20);
            void* subObject = reinterpret_cast<uint8_t*>(a2) + 0x20;
            uint8_t* entry = reinterpret_cast<uint8_t*>(
                reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    reinterpret_cast<void**>(subVtblPtr)[9])(subObject, nullptr));

            if (entry)
            {
                CSF_AT(uint32_t, self, 0x110) += CSF_AT(uint32_t, entry, 8);
            }
        }
    }

    return processed;
}

HOOK(0x006C67C0, C_008656d0_ProcessSkinnedItemCount, "auto");
