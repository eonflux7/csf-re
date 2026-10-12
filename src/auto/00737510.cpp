#include <cstdint>
#include "globals.h"
#include <cstring>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Caches a pair of 32-bit integers (x, y) at offsets 0x3c/0x40. When the pair differs from the cached copy,
// it sets a dirty flag at 0x20, stores the new pair, and stores both values again as floats at 0x44/0x48.
// Returns the input pointer when unchanged; when changed, returns the bit pattern of the float x value.
static uint32_t* __fastcall C_00867260_CacheIntPairAndFloats(C_00867260* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t* result = a2;

    if (CSF_AT(uint32_t, self, 0x3c) != a2[0] || CSF_AT(uint32_t, self, 0x40) != a2[1])
    {
        CSF_AT(uint8_t, self, 0x20) = 1;
        CSF_AT(uint32_t, self, 0x3c) = a2[0];
        CSF_AT(uint32_t, self, 0x40) = a2[1];

        float floatX = static_cast<float>(static_cast<int32_t>(a2[0]));
        float floatY = static_cast<float>(static_cast<int32_t>(a2[1]));

        CSF_AT(float, self, 0x44) = floatX;
        CSF_AT(float, self, 0x48) = floatY;

        uint32_t floatXBits = 0;
        std::memcpy(&floatXBits, &floatX, sizeof(floatXBits));
        return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(floatXBits));
    }

    return result;
}

HOOK(0x00737510, C_00867260_CacheIntPairAndFloats, "auto");
