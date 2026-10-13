#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Appends one vertex (position xyz + packed color) to the vertex buffer at self+4.
// Vertex stride is 36 bytes. Color bytes are packed as in the original:
//   packed = c[2] | (c[1] << 8) | (c[0] << 16) | (c[3] << 24)
static int __fastcall SceneLoader_AddColoredVertex(void* self, void* /*edx*/,
                                                   const uint32_t* position,
                                                   const uint8_t* color)
{
    uint8_t* vertices = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, self, 0x4)));
    uint32_t index = CSF_AT(uint32_t, self, 0xC);
    uint8_t* vertex = vertices + 36 * index;

    CSF_AT(uint32_t, vertex, 0x00) = position[0];
    CSF_AT(uint32_t, vertex, 0x04) = position[1];
    CSF_AT(uint32_t, vertex, 0x08) = position[2];

    uint16_t lowWord = static_cast<uint16_t>(color[0] | (static_cast<uint16_t>(color[3]) << 8));
    uint32_t packedColor = static_cast<uint32_t>(color[2])
                         | ((static_cast<uint32_t>(color[1]) | (static_cast<uint32_t>(lowWord) << 8)) << 8);
    CSF_AT(uint32_t, vertex, 0x18) = packedColor;

    int result = static_cast<int>(CSF_AT(uint32_t, self, 0xC)) + 1;
    CSF_AT(uint8_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0xC) = static_cast<uint32_t>(result);
    return result;
}

HOOK(0x6C3630, SceneLoader_AddColoredVertex, "auto");
