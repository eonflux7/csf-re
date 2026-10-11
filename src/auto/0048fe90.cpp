#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall C_00833908_GetTargetName(C_00833908* self, void* /*edx*/)
{
    auto* target = reinterpret_cast<uint8_t*>(self->field_002c);
    const uint32_t capacity = CSF_AT(uint32_t, target, 0x38);
    auto* name = target + 0x20;
    if (capacity < 0x10)
        return static_cast<int>(reinterpret_cast<uintptr_t>(name + 4));
    return CSF_AT(int, name, 0x04);
}

HOOK(0x0048FE90, C_00833908_GetTargetName, "auto");
