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
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destructor-like reset of C_00840ca8. Sets the derived vtable, releases the owned
// heap block at +0x18, clears the three members, restores the base vtable, and frees
// the object itself when bit 0 of a2 is set. Returns this.
// __thiscall with one stack arg: callee pops 4 bytes (ret 4).
static void** __fastcall C_00840ca8_Destroy(C_00840ca8* self, void* /*edx*/, char a2)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00840CA8);

    if (self->field_0018) {
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0018));
    }
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00830438);

    if ((a2 & 1) != 0) {
        CoreMemory_Free(self);
    }
    return reinterpret_cast<void**>(self);
}

HOOK(0x519360, C_00840ca8_Destroy, "auto");
