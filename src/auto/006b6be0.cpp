#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns the surface flag of a triangle. Each vertex index addresses a 4-byte
// entry in the geometry's table at +0x30; the result is the average of the nine
// bytes of the three entries (integer division by 9). If the object's flag
// field is set, the byte at +7 of the index array is returned instead.
static char __fastcall S_006b6be0_GetTriangleSurfaceFlag(S_006b6be0* self, void* /*edx*/,
                                                         uint16_t* vertexIndices, int geometry)
{
    if (self->field_0027)
    {
        return static_cast<char>(reinterpret_cast<uint8_t*>(vertexIndices)[7]);
    }

    void* geometryObj = reinterpret_cast<void*>(geometry);
    uint32_t tableAddr = CSF_AT(uint32_t, geometryObj, 0x30);
    if (tableAddr == 0)
    {
        return static_cast<char>(0xFF);
    }

    const uint8_t* table = reinterpret_cast<const uint8_t*>(tableAddr);
    uint32_t vertex0 = vertexIndices[0];
    uint32_t vertex1 = vertexIndices[1];
    uint32_t vertex2 = vertexIndices[2];

    int sum = table[4 * vertex1 + 1] + table[4 * vertex1 + 2]
            + table[4 * vertex0] + table[4 * vertex0 + 1] + table[4 * vertex0 + 2]
            + table[4 * vertex1]
            + table[4 * vertex2] + table[4 * vertex2 + 1] + table[4 * vertex2 + 2];

    return static_cast<char>(sum / 9);
}

HOOK(0x006B6BE0, S_006b6be0_GetTriangleSurfaceFlag, "auto");
