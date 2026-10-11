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

// Entry table (self+0x0C): array of 28-byte records.
// Record layout as used here:
//   +0x00  callback object pointer (vtable slot 17 = +0x44 is called with a delta vector)
//   +0x08  pointer to float[3] source vector
//   +0x10  accumulated X, +0x14 accumulated Y, +0x18 accumulated Z
// Adds a3 * source vector into the record's accumulators, then, if the record
// has a callback object, calls its slot 17 with the scaled delta and returns
// that result. Otherwise returns the record's address.
static int __fastcall EntryList_AddScaledVectorAndNotify(uint8_t* self, void* /*edx*/, int a2, float a3)
{
    uint32_t entryAddr = CSF_AT(uint32_t, self, 0x0C) + static_cast<uint32_t>(28 * a2);
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryAddr));

    float* vec = reinterpret_cast<float*>(static_cast<uintptr_t>(CSF_AT(uint32_t, entry, 0x08)));
    float vx = vec[0];
    float vy = vec[1];
    float vz = vec[2];

    float scaledX = vx * a3;
    float scaledY = vy * a3;
    float scaledZ = vz * a3;

    CSF_AT(float, entry, 0x10) = scaledX + CSF_AT(float, entry, 0x10);
    CSF_AT(float, entry, 0x14) = scaledY + CSF_AT(float, entry, 0x14);
    CSF_AT(float, entry, 0x18) = scaledZ + CSF_AT(float, entry, 0x18);

    uint32_t target = CSF_AT(uint32_t, entry, 0x00);
    if (target == 0)
    {
        return static_cast<int>(entryAddr);
    }

    float delta[3] = { scaledX, scaledY, scaledZ };
    void* obj = reinterpret_cast<void*>(static_cast<uintptr_t>(target));
    using NotifyFn = int(__fastcall*)(void* self, void* edx, float* vec);
    NotifyFn notify = reinterpret_cast<NotifyFn>(CSF_AT(void**, obj, 0)[68 / 4]);
    return notify(obj, nullptr, delta);
}

HOOK(0x004082E0, EntryList_AddScaledVectorAndNotify, "auto");
