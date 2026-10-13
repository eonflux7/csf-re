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

// Pointer to the kernel CloseHandle import (stdcall, one HANDLE argument).
CSF_GLOBAL(void*, g_pfnCloseHandle, 0x0082D0E0);

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destructor for C_0085eb30: closes the owned handle, resets the vtable to the
// base class, and frees the object when the low bit of `releaseMemory` is set.
static uint32_t* __fastcall C_0085eb30_Destructor(C_0085eb30* self, void* /*edx*/, char releaseMemory)
{
    uint32_t ownedHandle = self->field_0004;
    self->vtbl = CSF_VTBL(0x0085EB30);
    // The original leaves ecx holding `this` (its entry value) when it calls the
    // stdcall CloseHandle import. Calling through a fastcall-typed pointer puts
    // `self` in ecx and the handle on the stack; the callee pops the 4 bytes,
    // matching the stdcall call.
    ((void(__fastcall*)(void*, void*, uint32_t))g_pfnCloseHandle)(self, nullptr, ownedHandle);
    self->field_0004 = 0;
    self->vtbl = CSF_VTBL(0x0085EB24);
    if ((releaseMemory & 1) != 0)
        CoreMemory_Free(self);
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0069E410, C_0085eb30_Destructor, "auto");
