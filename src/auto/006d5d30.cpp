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

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(void, __fastcall, C_00860894_DestroyEmbeddedMember8, 0x006C4A50, (C_00860894* self, void*));

static int __fastcall C_008305f8_Destroy(C_008305f8* self, void* /*edx*/, char freeFlag)
{
    // While the embedded member is torn down, the object temporarily presents
    // the vtable 0x861080.
    *(void***)self = CSF_VTBL(0x00861080);
    C_00860894_DestroyEmbeddedMember8(
        reinterpret_cast<C_00860894*>(reinterpret_cast<uint8_t*>(self) + 8), nullptr);

    *(void***)self = CSF_VTBL(0x008305F8);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);

    if ((freeFlag & 1) != 0)
        CoreMemory_Free(self);

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x006D5D30, C_008305f8_Destroy, "auto");
