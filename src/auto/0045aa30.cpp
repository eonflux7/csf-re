#include <cstdint>
#include "globals.h"
#include "hook.h"

// Copies the second and third dwords of the pointed-to record into this+0x8 and this+0xC.
// Original: __thiscall, one stack arg, ret 4. Returns the third dword (left in eax).
static int __fastcall HudStatusIcons_CopySlotHandlePair(void* self, void* /*edx*/, uint32_t* a2) {
    CSF_AT(uint32_t, self, 0x8) = a2[1];
    int result = static_cast<int>(a2[2]);
    CSF_AT(uint32_t, self, 0xC) = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x45AA30, HudStatusIcons_CopySlotHandlePair, "auto");
