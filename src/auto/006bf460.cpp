#include <cstdint>
#include "types/auto_classes.h"
#include "hook.h"

// Returns the value of field_000c plus 64.
static int __fastcall C_00860700_GetField000cPlus64(C_00860700* self, void* /*edx*/)
{
    return static_cast<int>(self->field_000c + 0x40);
}

HOOK(0x006BF460, C_00860700_GetField000cPlus64, "auto");
