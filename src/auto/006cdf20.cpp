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

// Probes one shape record (passed by address) and reports whether it is usable.
CSF_FUNC(bool, __fastcall, GfxCollisionShapes_ProbeShape, 0x006B2620, (void* self, void* edx, int a2));

// Scans the shape table in fixed 100-byte records and returns the index of the first
// record that passes the probe and has a nonzero flag at +0x60, or -1 if none does.
static int __fastcall GfxCollisionShapes_FindFirstLiveShapeIndex(C_00860c88* self, void* /*edx*/, char* a2)
{
    uint32_t shapeIndex = 0;
    int byteOffset = 0;

    for (;; byteOffset += 100)
    {
        uint32_t tableBegin = CSF_AT(uint32_t, self, 0x8);
        if (!tableBegin)
            break;

        uint32_t tableEnd = CSF_AT(uint32_t, self, 0xC);
        int recordCount = static_cast<int>(tableEnd - tableBegin) / 100;
        if (shapeIndex >= static_cast<uint32_t>(recordCount))
            break;

        bool passesProbe = GfxCollisionShapes_ProbeShape(
            reinterpret_cast<void*>(a2), nullptr, static_cast<int>(tableBegin + byteOffset));

        if (passesProbe)
        {
            // Table base is re-read after the probe call, as the original does.
            uint32_t liveBegin = CSF_AT(uint32_t, self, 0x8);
            uint8_t* record = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(liveBegin + byteOffset));
            if (CSF_AT(int, record, 0x60) != 0)
                return static_cast<int>(shapeIndex);
        }

        ++shapeIndex;
    }

    return -1;
}

HOOK(0x006CDF20, GfxCollisionShapes_FindFirstLiveShapeIndex, "auto");
