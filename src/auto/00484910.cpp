#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, CCharActionFsm_PrepareDispatch, 0x004846D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, FlagHolder_SetMaskBits, 0x00498F50, (void* self, void* edx, int a2, char a3));

static int __fastcall CCharActionFsm_UpdateFlagsAndDispatch(CCharActionFsm* self, void* /*edx*/)
{
    // Clear bit 0x40000 in the flag word at +0x2E4 of the object held in field_0008.
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x2e4) &= ~0x40000u;
    FlagHolder_SetMaskBits(reinterpret_cast<void*>(self->field_0008), nullptr, 0x40000, 0);

    if (self->field_000c == 12)
    {
        CCharActionFsm_PrepareDispatch(self, nullptr);
        if (self->field_00a0)
            return self->vSlot138(16);
        return self->vSlot134(2, 0);
    }

    // Set bit 0x100 in the same flag word (re-read field_0008 as the original does).
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x2e4) |= 0x100u;
    self->field_00a4 = self->field_008c;
    return self->vSlot134(12, 0);
}

HOOK(0x484910, CCharActionFsm_UpdateFlagsAndDispatch, "auto");
