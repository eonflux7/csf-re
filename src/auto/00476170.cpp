#include <cstdint>
#include "globals.h"
#include "hook.h"

static void __fastcall WeaponFirearms_SetEntry(void* self, void* /*edx*/, void* entry)
{
    CSF_AT(void*, self, 0x1C) = entry;
}

HOOK(0x00476170, WeaponFirearms_SetEntry, "auto");
