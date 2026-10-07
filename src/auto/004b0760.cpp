#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// The original is a __thiscall method taking one (unused) integer argument,
// which stores 0 into the 4-byte member at offset +0x4 of the object.
static void __fastcall C_Unknown_ClearField004(void* self, void* /*edx*/, int /*unused*/)
{
    CSF_AT(uint32_t, self, 0x4) = 0;
}

HOOK(0x004B0760, C_Unknown_ClearField004, "auto");
