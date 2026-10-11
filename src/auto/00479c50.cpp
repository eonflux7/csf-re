#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, WeaponFirearms_Update, 0x004789D0,
               (void* self, void* edx, float a2));

static int __fastcall C_008318c8_Update(C_008318c8* self, void* /*edx*/, float a2)
{
    return WeaponFirearms_Update(self, nullptr, a2);
}

HOOK(0x00479C50, C_008318c8_Update, "auto");
