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

// Calls the virtual method at vtable slot 71 (offset 0x11c) with argument 16.
static int __fastcall CCharWeaponFsm_InvokeVtblSlot71With16(CCharWeaponFsm* self, void* /*edx*/)
{
    return ((int(__fastcall*)(void*, void*, int))self->vtbl[0x11c / 4])(self, nullptr, 16);
}

HOOK(0x00480670, CCharWeaponFsm_InvokeVtblSlot71With16, "auto");
