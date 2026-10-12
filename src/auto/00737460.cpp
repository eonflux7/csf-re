#include <cstdint>
#include "globals.h"
#include <cstring>

#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Caches a pair of 32-bit integers (x, y) on the object. When the pair differs from the cached copy,
// it sets a dirty flag, stores the new pair, and stores both values again as floats.
// Returns the input pointer when unchanged; when changed, returns the bit pattern of the float x value.
static uint32_t* __fastcall C_00867260_UpdateIntPairCache(C_00867260* self, void* /*edx*/, uint32_t* a2)
{
    uint32_t* result = a2;

    if (CSF_AT(uint32_t, self, 0x24) != a2[0] || CSF_AT(uint32_t, self, 0x28) != a2[1])
    {
        CSF_AT(uint8_t, self, 0x20) = 1;
        CSF_AT(uint32_t, self, 0x24) = a2[0];
        CSF_AT(uint32_t, self, 0x28) = a2[1];

        float floatX = static_cast<float>(static_cast<int32_t>(a2[0]));
        float floatY = static_cast<float>(static_cast<int32_t>(a2[1]));

        CSF_AT(float, self, 0x2c) = floatX;
        CSF_AT(float, self, 0x30) = floatY;

        uint32_t floatXBits = 0;
        std::memcpy(&floatXBits, &floatX, sizeof(floatXBits));
        return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(floatXBits));
    }

    return result;
}

HOOK(0x00737460, C_00867260_UpdateIntPairCache, "auto");
