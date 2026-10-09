#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Releases the weapon record storage of CWeaponDefTable (sub_784530).
// Return value is not used by the destructor, but the call must be kept.
CSF_FUNC(void, __fastcall, CWeaponDefTable_ReleaseRecords, 0x00784530, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for CWeaponDefTable: releases the records, then frees
// the object through the game heap when the low bit of the flags argument is set.
static int* __fastcall CWeaponDefTable_Destroy(void* self, void* /*edx*/, char flags)
{
    CWeaponDefTable_ReleaseRecords(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<int*>(self);
}

HOOK(0x7848D0, CWeaponDefTable_Destroy, "auto");
