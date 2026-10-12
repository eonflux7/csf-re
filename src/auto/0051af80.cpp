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

// Original game heap free (debug variant), cdecl, one block pointer.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Base-class cleanup: releases the record block owned by CWeaponDefTable.
CSF_FUNC(void, __fastcall, CWeaponDefTable_ReleaseRecordBlock, 0x00784530, (CWeaponDefTable* self, void*));

// CWeaponDatabase destructor-style cleanup (vtable 0x840D88).
// Frees both heap arrays, clears their pointers, then runs the base-class release.
static int __fastcall CWeaponDatabase_Cleanup(CWeaponDatabase* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00840D88);

    // Second array (+0x20 / +0x24 / +0x28).
    if (self->m_pArray2)
        Mem_FreeDebug(reinterpret_cast<void*>(self->m_pArray2));
    self->m_pArray2 = 0;
    self->m_pArray2End = 0;
    self->m_pArray2Cap = 0;

    // Per-weapon flag bytes (+0x10 / +0x14 / +0x18).
    if (self->m_pWeaponFlags)
        Mem_FreeDebug(reinterpret_cast<void*>(self->m_pWeaponFlags));
    self->m_pWeaponFlags = 0;
    self->m_pWeaponFlagsEnd = 0;
    self->m_pWeaponFlagsCap = 0;

    CWeaponDefTable_ReleaseRecordBlock(reinterpret_cast<CWeaponDefTable*>(self), nullptr);
    return 0;
}

HOOK(0x0051AF80, CWeaponDatabase_Cleanup, "auto");
