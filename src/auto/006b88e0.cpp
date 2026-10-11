#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global base pointer (dword_8BB484) used as the origin for the property value slot.
CSF_GLOBAL(uint8_t*, g_PropertyValueBase, 0x8BB484);

// Stores `value` as a 32-bit integer at (base + offset) and returns `value`.
static int __cdecl GfxObjectProps_StorePropertyValueAtOffset(int offset, int value)
{
    *reinterpret_cast<int*>(g_PropertyValueBase + offset) = value;
    return value;
}

HOOK(0x006B88E0, GfxObjectProps_StorePropertyValueAtOffset, "auto");
