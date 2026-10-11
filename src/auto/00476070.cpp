#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall WeaponFirearms_GetOwnerValue(void* self, void* /*edx*/)
{
    void* owner = CSF_AT(void*, self, 0x40);
    return CSF_AT(int, owner, 0x104);
}

HOOK(0x00476070, WeaponFirearms_GetOwnerValue, "auto");
