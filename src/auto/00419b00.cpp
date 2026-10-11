#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, sub_418E50, 0x00418E50, (void* self, void* edx));

// Copies the 4-dword vector at a2 into the object's +0x20..+0x2C block, marks the
// object dirty (+0x85 = 1), refreshes it via sub_418E50, then calls the virtual at
// vtable slot 0x60. Does nothing when bit 2 of +0x7C is set.
static int __fastcall C_0082e1e8_SetTransformFromArray(C_0082e1e8* self, void* /*edx*/, uint32_t* a2)
{
    int result = 0;

    if ((self->field_007c & 2) == 0)
    {
        self->field_0020 = a2[0];
        CSF_AT(uint32_t, self, 0x24) = a2[1];
        CSF_AT(uint32_t, self, 0x28) = a2[2];
        CSF_AT(uint32_t, self, 0x2C) = a2[3];
        self->field_0085 = 1;
        sub_418E50(self, nullptr);
        return self->vSlot60();
    }
    return result;
}

HOOK(0x419B00, C_0082e1e8_SetTransformFromArray, "auto");
