#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, sub_527DC0, 0x00527DC0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_4DE9B0, 0x004DE9B0, (void* self, void* edx, int a2));

// Constructor for C_00841128: resets the sound-instance fields and runs the
// 0x14-entry initialisation on the sub-object at +0x70 when the count is below 20.
static uint32_t* __fastcall C_00841128_Construct(C_00841128* self, void* /*edx*/)
{
    sub_527DC0(self, nullptr);

    *reinterpret_cast<void**>(self) = CSF_VTBL(0x00841128);
    self->field_0070 = 0;
    CSF_AT(uint32_t, self, 0x74) = 0;
    CSF_AT(uint32_t, self, 0x78) = 0;
    CSF_AT(uint32_t, self, 0x7C) = 10;

    uint32_t filledCount = 0;
    if (filledCount < 0x14u) {
        void* slotBlock = reinterpret_cast<uint8_t*>(self) + 0x70;
        sub_4DE9B0(slotBlock, nullptr, static_cast<int>(0x14u - filledCount));
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x527FE0, C_00841128_Construct, "auto");
