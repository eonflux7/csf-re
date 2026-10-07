#include <cstdint>
#include "globals.h"
#include "types/auto_classes.h"
#include "core/memory.h"
#include "hook.h"

// Returns the dword stored at offset +0x0C of the object referenced by
// this->field_00bc.
static int __fastcall C_00860bb8_GetInnerObjectField0C(C_00860bb8* self, void* /*edx*/)
{
    void* inner = reinterpret_cast<void*>(self->field_00bc);
    return static_cast<int>(CSF_AT(uint32_t, inner, 0xc));
}

HOOK(0x006CB940, C_00860bb8_GetInnerObjectField0C, "auto");
