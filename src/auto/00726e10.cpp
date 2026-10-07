// Reimplementation of sub_726E10 (0x00726E10)
// Sets byte at +0x68 to 1, clears dword at +0x60, and returns the address of +0x58.

#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// __thiscall: ecx = this, no stack arguments (ret 0)
static int __fastcall C_008655e8_SetFlag68Clear60Get58(C_008655e8* self, void* /*edx*/)
{
    self->field_0068 = 1;
    self->field_0060 = 0;
    return static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x58));
}

HOOK(0x00726E10, C_008655e8_SetFlag68Clear60Get58, "auto");
