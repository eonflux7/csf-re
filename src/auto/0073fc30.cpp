// NetCogibleDDL.cpp (generated DDL) - C_008677c0 reset: rebinds vtables, frees the
// spawn list, releases the 0x8C list's extra node, then chains to the base-class reset.
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

CSF_FUNC(uint32_t*, __fastcall, ClearListAndFreeNodes, 0x004E9980, (uint32_t* self, void*));
CSF_FUNC(void, __fastcall, sub_740840, 0x00740840, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void, __cdecl, MpSpawnPoints_FinishEventDescriptor, 0x0040ECF0, ());
CSF_FUNC(int, __fastcall, sub_64EBC0, 0x0064EBC0, (void* self, void* edx));

static int __fastcall C_008677c0_ReleaseSpawnListAndRebase(C_008677c0* self, void* /*edx*/)
{
    // Derived vtable first.
    self->vtbl = CSF_VTBL(0x008677C0);

    // Sub-object at +0x70 gets its own vtable.
    CSF_AT(void*, self, 0x70) = CSF_VTBL(0x008677B0);

    // List at +0x8C: clear nodes, then free the extra block held at +0x90.
    uint8_t* listBase = reinterpret_cast<uint8_t*>(self) + 0x8C;
    ClearListAndFreeNodes(reinterpret_cast<uint32_t*>(listBase), nullptr);

    void* extraBlock = CSF_AT(void*, self, 0x90);
    if (extraBlock)
        Mem_FreeDebug(extraBlock);
    CSF_AT(void*, self, 0x90) = nullptr;

    // Sub-object at +0x70 reset.
    sub_740840(reinterpret_cast<uint8_t*>(self) + 0x70, nullptr);

    // Restore base-class vtable.
    self->vtbl = CSF_VTBL(0x00867770);

    // Spawn-point descriptor at +0x44.
    MpSpawnPoints_FinishEventDescriptor();

    // Tail call to base reset; forwards its return value.
    return sub_64EBC0(self, nullptr);
}

HOOK(0x0073FC30, C_008677c0_ReleaseSpawnListAndRebase, "auto");
