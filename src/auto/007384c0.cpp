#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the two-dword pair at a2 into this->field_0014/field_0018 when either
// differs from the stored values, flagging field_0010. Returns a2 when unchanged,
// otherwise the new second dword (as the original does).
static uint32_t* __fastcall C_008672e4_CopyPairAt0014IfChanged(C_008672e4* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t* result = a2;
    if (self->field_0014 != a2[0] || self->field_0018 != a2[1]) {
        self->field_0010 = 1;
        self->field_0014 = a2[0];
        result = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(a2[1]));
        self->field_0018 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(result));
    }
    return result;
}

HOOK(0x7384C0, C_008672e4_CopyPairAt0014IfChanged, "auto");
