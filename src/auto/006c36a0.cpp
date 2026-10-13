#include <cstdint>

#include "globals.h"
#include "hook.h"

// Appends one triangle vertex (position, packed color, UV pair) to a particle
// system definition's vertex array and bumps the vertex count.
//   self+0x04: vertex array (36-byte records)
//   self+0x0C: vertex count
//   self+0x2C: dirty flag
static int __fastcall GfxParticleSystemDefs_AppendTriangleVertex(
    void* self, void* /*edx*/, uint32_t* position, uint8_t* color, uint32_t* uv) {
    // Record at the current end of the vertex array.
    const int vertexIndex = CSF_AT(int, self, 0x0C);
    uint8_t* vertexRecord = CSF_AT(uint8_t*, self, 0x04) + 36 * vertexIndex;

    // Position (x, y, z).
    CSF_AT(uint32_t, vertexRecord, 0x00) = position[0];
    CSF_AT(uint32_t, vertexRecord, 0x04) = position[1];
    CSF_AT(uint32_t, vertexRecord, 0x08) = position[2];

    // Color bytes packed as 0xAARRGGBB-style: b3 << 24 | b0 << 16 | b1 << 8 | b2.
    const uint32_t packedColor =
        (static_cast<uint32_t>(color[3]) << 24) |
        (static_cast<uint32_t>(color[0]) << 16) |
        (static_cast<uint32_t>(color[1]) << 8) |
        static_cast<uint32_t>(color[2]);
    CSF_AT(uint32_t, vertexRecord, 0x18) = packedColor;

    // UV pair.
    CSF_AT(uint32_t, vertexRecord, 0x1C) = uv[0];
    CSF_AT(uint32_t, vertexRecord, 0x20) = uv[1];

    const int newCount = CSF_AT(int, self, 0x0C) + 1;
    CSF_AT(uint8_t, self, 0x2C) = 1;
    CSF_AT(int, self, 0x0C) = newCount;
    return newCount;
}

HOOK(0x006C36A0, GfxParticleSystemDefs_AppendTriangleVertex, "auto");
