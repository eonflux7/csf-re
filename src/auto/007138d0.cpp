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

// Game heap free (0x691110, __cdecl, pops nothing).
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// CInputBase cleanup, called from the CInputDirectInput destructor (__thiscall, no args).
CSF_FUNC(int, __fastcall, CInputBase_ReleaseResources, 0x00712F00, (void* self, void* edx));

// CInputDirectInput destructor (__thiscall, ret 4): the flag byte selects whether the object itself is freed.
static int __fastcall CInputDirectInput_DestroyAndMaybeFree(CInputDirectInput* self, void* /*edx*/, char a2)
{
    *(void***)self = CSF_VTBL(0x00863ED8);
    CInputBase_ReleaseResources(self, nullptr);
    *(void***)self = CSF_VTBL(0x00863E58);

    if (self->field_0008)
    {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0008));
        self->field_0008 = 0;
    }

    if ((a2 & 1) != 0)
    {
        CoreMemory_Free(self);
    }

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x7138D0, CInputDirectInput_DestroyAndMaybeFree, "auto");
