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

// Game debug-heap free for the flag arrays.
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
// Game heap free used for the object itself when the delete flag is set.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
// Base-class record-block release (CWeaponDefTable), __thiscall.
CSF_FUNC(void, __fastcall, CWeaponDefTable_ReleaseRecordBlock, 0x00784530, (CWeaponDefTable* self, void*));

// Releases the two heap arrays owned by CWeaponDatabase, the base record block,
// and optionally frees the object itself (a2 bit 0). Returns this.
static void** __fastcall CWeaponDatabase_ReleaseArraysAndMaybeFree(CWeaponDatabase* self, void* /*edx*/, char releaseFlags)
{
    self->vtbl = CSF_VTBL(0x00840D88);

    if (self->m_pArray2 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->m_pArray2));
    self->m_pArray2 = 0;
    self->m_pArray2End = 0;
    self->m_pArray2Cap = 0;

    if (self->m_pWeaponFlags != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->m_pWeaponFlags));
    self->m_pWeaponFlags = 0;
    self->m_pWeaponFlagsEnd = 0;
    self->m_pWeaponFlagsCap = 0;

    CWeaponDefTable_ReleaseRecordBlock(reinterpret_cast<CWeaponDefTable*>(self), nullptr);

    if ((releaseFlags & 1) != 0)
        Mem_Free(self);

    return reinterpret_cast<void**>(self);
}

HOOK(0x0051B060, CWeaponDatabase_ReleaseArraysAndMaybeFree, "auto");
