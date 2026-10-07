#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0044F680 (8 bytes, ret 0) — writes 1 to the byte field at this+0x128.
// This is a __thiscall member with no stack arguments (ecx = this, edx unused);
// written as a free __fastcall so the callee pops 0 stack bytes, matching `ret`.
static void __fastcall C_0044f680_SetFlag128(void* self, void* /*edx*/)
{
    CSF_AT(uint8_t, self, 0x128) = 1;
}

HOOK(0x44F680, C_0044f680_SetFlag128, "auto");
