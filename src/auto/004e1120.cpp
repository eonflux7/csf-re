#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Bind as given by the packet.
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

// __thiscall method written as a free function: ecx = self, edx unused.
static int __fastcall C_0083e5e8_ComputeScaledLinkedValueAtLeastTwo(C_0083e5e8* self, void* /*edx*/)
{
    // Global block that holds the scale factor at +0x1828.
    uint8_t* globalValueBase = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());

    // Linked value from the virtual slot +0x84 (vGetLinkedValueDC).
    int linkedValue = self->vGetLinkedValueDC();

    // Read the field as a signed int; if negative, treat it as unsigned
    // by adding 2^32 (g_UnsignedIntegerBias).
    int rawValue = CSF_AT(int, globalValueBase, 0x1828);
    float unsignedValue = static_cast<float>(rawValue);
    if (rawValue < 0) {
        unsignedValue += g_UnsignedIntegerBias;
    }

    float product = static_cast<float>(linkedValue) * g_ProgressScale * unsignedValue;

    // Truncate toward zero to a 64-bit integer; the result is used as its low 32 bits.
    int32_t scaled = static_cast<int32_t>(static_cast<int64_t>(product));

    // Clamp to a minimum of 2.
    if (scaled > 1) {
        return scaled;
    }
    return 2;
}

HOOK(0x004E1120, C_0083e5e8_ComputeScaledLinkedValueAtLeastTwo, "auto");
