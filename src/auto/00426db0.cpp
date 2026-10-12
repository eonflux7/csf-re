#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ReserveSegmentStorage, 0x00425CE0,
         (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, S_00426550_BuildSegments, 0x00426550,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_00426550_PrepareSegments, 0x00426A20,
         (void* self, void* edx));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

// The packet also classifies the BuildSegments call target as a vtable.
[[maybe_unused]] static const auto kBuildSegmentsTable = CSF_VTBL(0x00426550);

static void __fastcall S_00426550_RebuildSegments(
    S_00426550* self, void* /*edx*/)
{
    if (self->field_001c > 2) {
        if (CSF_AT(uint8_t, self, 0x28)) {
            S_00426550_PrepareSegments(self, nullptr);
            self->field_0024 = 2;
        }
        S_00426550_BuildSegments(self, nullptr);
        return;
    }

    const auto* points = reinterpret_cast<const uint8_t*>(self->field_0014);
    const float startX = CSF_AT(float, points, 0x00);
    const float startZ = CSF_AT(float, points, 0x08);
    const float deltaX = CSF_AT(float, points, 0x14) - startX;
    const float deltaZ = CSF_AT(float, points, 0x1C) - startZ;

    alignas(8) uint8_t segmentBytes[0x40]{};
    CSF_AT(uint32_t, segmentBytes, 0x00) = 1;
    CSF_AT(float, segmentBytes, 0x08) = startX;
    CSF_AT(float, segmentBytes, 0x0C) = startZ;
    CSF_AT(float, segmentBytes, 0x10) = deltaX;
    CSF_AT(float, segmentBytes, 0x14) = deltaZ;

    self->field_000c = 0;
    const uint32_t capacity = CSF_AT(uint32_t, self, 0x08);
    const uint32_t count = self->field_000c;

    const float length = std::sqrt(deltaZ * deltaZ + deltaX * deltaX);
    CSF_AT(float, segmentBytes, 0x04) = length;
    const float inverseLength = g_TransitionOne / length;
    CSF_AT(float, segmentBytes, 0x10) = deltaX * inverseLength;
    CSF_AT(float, segmentBytes, 0x14) = inverseLength * deltaZ;

    if (count >= capacity) {
        ReserveSegmentStorage(reinterpret_cast<uint8_t*>(self) + 0x04,
                              nullptr, CSF_AT(int, self, 0x10));
    }

    const uint32_t insertionIndex = self->field_000c;
    auto* storage = reinterpret_cast<uint8_t*>(self->field_0004);
    self->field_000c = insertionIndex + 1;
    std::memcpy(storage + 36 * insertionIndex, segmentBytes, 0x24);
}

HOOK(0x00426DB0, S_00426550_RebuildSegments, "auto");
