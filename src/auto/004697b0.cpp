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

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Destructor-like reset of C_008314c0 (mission script parser node).
// Clears the owned heap block and counters, restores the base vtable,
// and frees the object itself when bit 0 of a2 is set.
// __thiscall with one stack arg: callee pops 4 bytes (ret 4).
static void** __fastcall C_008314c0_Destroy(C_008314c0* self, void* /*edx*/, char a2)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x008314C0);

    if (self->field_0008) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0008));
    }
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);

    if ((a2 & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<void**>(self);
}

HOOK(0x4697B0, C_008314c0_Destroy, "auto");
