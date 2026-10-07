#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x005274F0 (12 bytes): returns (self->field_006c == 2)
static int __fastcall C_008410b0_IsField006cEqual2(C_008410b0* self, void* /*edx*/)
{
    return self->field_006c == 2;
}

HOOK(0x5274F0, C_008410b0_IsField006cEqual2, "auto");
