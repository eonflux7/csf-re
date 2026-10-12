#include <cstdint>
#include "hook.h"

// Copies the first dword of the source block into the first dword of the
// destination and returns the destination (__thiscall, one stack argument).
static uint32_t* __fastcall GfxSkin_ConstructLookupKey(uint32_t* self, void* /*edx*/, const uint32_t* source) {
    self[0] = source[0];
    return self;
}

HOOK(0x006B27D0, GfxSkin_ConstructLookupKey, "auto");
