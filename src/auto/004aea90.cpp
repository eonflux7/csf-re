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

CSF_FUNC(void, __fastcall, C_0082d410_SetVtable, 0x004DBD40, (C_0082d410* self, void* edx));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destroys the three embedded sub-objects (at +0x08, +0x04 and +0x00) in that
// order, then frees the object itself through the game heap when bit 0 of
// a2 is set. Returns this.
static C_0082d410* __fastcall C_0082d410_DestroySubobjectsAndMaybeFree(C_0082d410* self, void* /*edx*/, char a2)
{
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x08), nullptr);
    C_0082d410_SetVtable(reinterpret_cast<C_0082d410*>(reinterpret_cast<uint8_t*>(self) + 0x04), nullptr);
    C_0082d410_SetVtable(self, nullptr);

    if ((a2 & 1) != 0)
        CoreMemory_Free(reinterpret_cast<void*>(self));

    return self;
}

HOOK(0x4AEA90, C_0082d410_DestroySubobjectsAndMaybeFree, "auto");
