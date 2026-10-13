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

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));

// Serializes a value that may be absent. The owner object is at [a2+4]; a2 itself holds
// the "absent" flag in pad_0000[0]. Returns 1 when the value is present, 0 when absent.
static int __fastcall Archive_SerializeOptionalCollectionB(void* self, void* /*edx*/, S_00691860* a2)
{
    bool isAbsent = false;
    if (!a2->pad_0000[0])
    {
        void* owner = reinterpret_cast<void*>(a2->field_0004);
        // vtable slot 0x80 / 4 = 32: bool(int, S_00691860*)
        isAbsent = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
                        CSF_AT(void**, owner, 0)[32])(owner, nullptr, 0, a2);
    }

    uint32_t savedCollection = CSF_AT(uint32_t, a2, 0x1c);
    a2->pad_0000[0] = isAbsent;
    CSF_AT(uint32_t, a2, 0x1c) = 0;

    if (!isAbsent)
    {
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(self), a2);
    }

    bool stillPresentFlag = (a2->pad_0000[0] == 0);
    CSF_AT(uint32_t, a2, 0x1c) = savedCollection;

    if (stillPresentFlag)
    {
        void* owner = reinterpret_cast<void*>(a2->field_0004);
        // vtable slot 0x7C / 4 = 31: bool(S_00691860*)
        if (((bool(__fastcall*)(void*, void*, S_00691860*))
                CSF_AT(void**, owner, 0)[31])(owner, nullptr, a2))
        {
            a2->pad_0000[0] = 0;
            return 0;
        }
    }

    a2->pad_0000[0] = 1;
    return 1;
}

HOOK(0x0044F150, Archive_SerializeOptionalCollectionB, "auto");
