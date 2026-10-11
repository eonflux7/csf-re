#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, C_00830030_InitBase, 0x00428B20, (void* self, void* edx));

// Initializes a C_008300c0 (cheats menu screen): runs the base init, sets the
// vtable, resets a 93-entry table of 20-byte records starting at +0x74C, then
// sets the two float fields at +0xE90/+0xE94 to 0.35f (0x3EB33333) and the flag at +0xE98 to 1.
static int __fastcall C_008300c0_Init(C_008300c0* self, void* /*edx*/)
{
    C_00830030_InitBase(self, nullptr);
    self->vtbl = CSF_VTBL(0x008300C0);

    uint8_t* cursor = reinterpret_cast<uint8_t*>(self) + 0x74C;
    int remaining = 93;
    do
    {
        CSF_AT(uint32_t, cursor, 0x00) = 0xFFFFFFFFu;
        CSF_AT(uint32_t, cursor, 0x04) = 0xFFFFFFFFu;
        CSF_AT(uint32_t, cursor, 0x08) = 0u;
        CSF_AT(uint32_t, cursor, 0x0C) = 0x3F800000u; // 1.0f
        CSF_AT(uint32_t, cursor, 0x10) = 0u;
        cursor += 20;
        --remaining;
    } while (remaining != 0);

    self->field_0e90 = 0;
    self->field_0e94 = 0;

    const uint32_t thresholdBits = 0x3EB33333u; // 0.35f
    self->field_0e90 = thresholdBits;
    self->field_0e94 = thresholdBits;
    self->field_0e98 = 1;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x00429360, C_008300c0_Init, "auto");
