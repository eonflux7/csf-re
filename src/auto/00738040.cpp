#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the two-dword pair at a2 into this->field_001c/field_0020 when either
// differs from the stored values, flagging field_0010. Returns a2 when unchanged,
// otherwise the new second dword (as the original does).
static uint32_t* __fastcall C_008672e4_CopyPairIfChanged(C_008672e4* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t* result = a2;
    if (self->field_001c != a2[0] || self->field_0020 != a2[1]) {
        self->field_0010 = 1;
        self->field_001c = a2[0];
        result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a2[1]));
        self->field_0020 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(result));
    }
    return result;
}

HOOK(0x738040, C_008672e4_CopyPairIfChanged, "auto");
