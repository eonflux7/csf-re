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

// Initializes the base part of a water object from a 6+ byte init record:
//   [0..3] -> field_0008, [4..7] -> field_000c, flag at [5] enables 0x40 bit on the sub-object at field_0004.
static int __fastcall C_008630e0_BaseInit(C_008630e0* self, void* /*edx*/, int a2)
{
    const void* record = reinterpret_cast<const void*>(static_cast<uintptr_t>(a2));

    self->field_0008 = CSF_AT(uint32_t, record, 0);
    self->field_000c = CSF_AT(uint32_t, record, 4);
    self->field_0010 = 1;

    // Virtual slot at vtable+0x10 (slot 4): no typed wrapper, so call through the vtable.
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x10 / 4])(self, nullptr);
    if (result == 0)
    {
        result = CSF_AT(uint8_t, record, 5);
        if (static_cast<uint8_t>(result) != 0)
        {
            uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0004));
            CSF_AT(uint32_t, owner, 8) |= 0x40u;
        }
    }
    return result;
}

HOOK(0x006FF510, C_008630e0_BaseInit, "auto");
