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

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, C_00863d80_ReleaseChildrenAndClearFlags, 0x00711960, (C_00863d80* self, void*));

// Destructor body for C_00863d80: resets the vtable to the derived class,
// releases the child state, drops the field to the base vtable, and frees
// the block when a2 bit 0 is set. Returns this (eax).
static uint32_t* __fastcall C_00863d80_DestroyAndMaybeFree(C_00863d80* self, void* /*edx*/, char a2)
{
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x00863D80);

    C_00863d80_ReleaseChildrenAndClearFlags(self, nullptr);

    self->field_0004 = 0;
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x00863D38);

    if ((a2 & 1) != 0) {
        CoreMemory_Free(self);
    }

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00712220, C_00863d80_DestroyAndMaybeFree, "auto");
