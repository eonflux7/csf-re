#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x00506D70 — __thiscall setter: writes the low byte of the single stack
// argument into the byte field at offset 0x20 of the object and returns it.
static char __fastcall SetByteAtOffset20(uint8_t* self, void* /*edx*/, char a2)
{
    CSF_AT(uint8_t, self, 0x20) = static_cast<uint8_t>(a2);
    return a2;
}

HOOK(0x506D70, SetByteAtOffset20, "auto");
