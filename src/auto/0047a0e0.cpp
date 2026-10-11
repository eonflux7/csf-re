#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008318c8_IsFlag40Set(C_008318c8* self, void* /*edx*/)
{
    return (CSF_AT(uint8_t, reinterpret_cast<void*>(self->field_0004), 0x2E0) & 0x40) != 0;
}

HOOK(0x0047A0E0, C_008318c8_IsFlag40Set, "auto");
