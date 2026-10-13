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

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, BddLoader_ReleasePendingHandle, 0x0050E250, (void* self, void*));
CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (C_0083b958* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Destructor for C_0083bc28 (vtable 0x83BC28, derived from C_0083b958).
// Restores the vtables, releases the pending BDD loader handle, runs the base
// destructor and frees the object when bit 0 of the argument is set.
static C_0083bc28* __fastcall C_0083bc28_DestructorWithOptionalFree(C_0083bc28* self, void* /*edx*/, char a2)
{
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x0083BC28);
    CSF_AT(uint32_t, self, 0x4) = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083BC08));

    int globalState = GetGlobalState_50D190();
    BddLoader_ReleasePendingHandle(reinterpret_cast<void*>(static_cast<uintptr_t>(globalState)), nullptr);

    CSF_AT(void*, self, 0x64) = CSF_VTBL(0x0082D410);

    C_0083b958_Destroy(reinterpret_cast<C_0083b958*>(self), nullptr);

    if ((a2 & 1) != 0)
        CoreMemory_Free(reinterpret_cast<void*>(self));

    return self;
}

HOOK(0x004D37E0, C_0083bc28_DestructorWithOptionalFree, "auto");
