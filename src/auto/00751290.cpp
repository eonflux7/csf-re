#include <cstdint>
#include "hook.h"

// 0x00751290 (6 bytes) — __thiscall, pops 0 stack bytes (plain ret).
// Stores the byte 1 at this+0x28 and returns 1 (AL).
static char __fastcall C_Unknown_SetActiveFlag(void* self, void* /*edx*/)
{
    // The owning class is unknown (raw _BYTE* in the decompilation), so the
    // byte field at +0x28 is written through a plain byte pointer.
    reinterpret_cast<uint8_t*>(self)[0x28] = 1;
    return 1;
}

HOOK(0x751290, C_Unknown_SetActiveFlag, "auto");
