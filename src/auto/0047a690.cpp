#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, PrepareWeaponState, 0x00477F90,
                (void* self, void* edx));
static CSF_FUNC(int, __fastcall, UpdateFirearmState, 0x0047A2E0,
                (void* self, void* edx));
static CSF_FUNC(unsigned int, __fastcall, GetFirearmUpdateResult, 0x0047A520,
                (void* self, void* edx));

static unsigned int __fastcall C_008318c8_UpdateAndGetResult(
    C_008318c8* self, void* /*edx*/)
{
    PrepareWeaponState(self, nullptr);
    UpdateFirearmState(self, nullptr);
    return GetFirearmUpdateResult(self, nullptr);
}

HOOK(0x0047A690, C_008318c8_UpdateAndGetResult, "auto");
