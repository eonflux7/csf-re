#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Original: 0x00519810, __fastcall (ecx=this), no stack args, returns unsigned int.
CSF_FUNC(unsigned int, __fastcall, C_00840d54_BaseCleanup, 0x00519810, (void* self, void* edx));

// Original: 0x00691110 (game heap free), __cdecl, one stack arg.
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Original: 0x00519880, __thiscall with one stack arg (a2), `ret 4`.
// Resets the vtable, runs base cleanup, frees the owned block, and optionally frees this object.
static unsigned int* __fastcall C_00840d54_Destroy(C_00840d54* self, void* /*edx*/, char a2)
{
    self->vtbl = CSF_VTBL(0x00840D54);
    C_00840d54_BaseCleanup(self, nullptr);

    if (self->field_0008)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0008));
        self->field_0008 = 0;
    }

    if ((a2 & 1) != 0)
    {
        CoreMemory_Free(self);
    }

    return reinterpret_cast<unsigned int*>(self);
}

HOOK(0x519880, C_00840d54_Destroy, "auto");
