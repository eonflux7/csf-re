#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_522BA0, 0x00522BA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_522C40, 0x00522C40, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, sub_523660, 0x00523660, (void* self, void* edx, int a2));

// Resets the two 4-slot counter blocks at +0x00 and +0x10/+0x20 to zero (cap 10),
// then tops up each block that is below 20 by calling its fill routine.
static uint32_t* __fastcall S_00522ba0_ResetAndTopUp(S_00522ba0* self, void* /*edx*/)
{
    self->field_0000 = 0;
    self->field_0004 = 0;
    self->field_0008 = 0;
    self->field_000c = 10;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 10;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2c) = 10;

    // Sub-object vtables referenced by the original's call sites (no store performed here).
    (void)CSF_VTBL(0x00522BA0);
    (void)CSF_VTBL(0x00523660);

    uint32_t countA = self->field_0004;
    if (countA < 20)
        sub_522BA0(self, nullptr, static_cast<int>(20 - countA));

    uint32_t countB = self->field_0014;
    if (countB < 20)
        sub_523660(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x10),
                   nullptr, static_cast<int>(20 - countB));

    // Re-read after the calls above, as the original does.
    uint32_t countC = CSF_AT(uint32_t, self, 0x24);
    if (countC < 20)
        sub_522C40(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x20),
                   nullptr, static_cast<int>(20 - countC));

    return &self->field_0000;
}

HOOK(0x524000, S_00522ba0_ResetAndTopUp, "auto");
