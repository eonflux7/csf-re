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

// Game heap free (cdecl), same binding as the original's call to Mem_Free.
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Indirect CloseHandle pointer stored in a global at 0x0082D0E0 (stdcall, one handle argument).
CSF_GLOBAL(void*, g_pfnCloseHandle, 0x0082D0E0);

// Destructor for C_0085ec74: releases its OS handle, restores the base vtable,
// and frees the block when the low bit of the flags argument is set.
static uint32_t* __fastcall C_0085ec74_Destroy(C_0085ec74* self, void* /*edx*/, char a2)
{
    uint32_t handleValue = self->field_0004;
    self->vtbl = CSF_VTBL(0x0085EC74);
    if (handleValue != 0xFFFFFFFFu)
    {
        reinterpret_cast<int(__stdcall*)(uint32_t)>(g_pfnCloseHandle)(handleValue);
        self->field_0004 = 0xFFFFFFFFu;
    }
    self->vtbl = CSF_VTBL(0x0085EC4C);
    if ((a2 & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x6A0500, C_0085ec74_Destroy, "auto");
