#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Frees a native handle owned by this object (original 0x81B9E0).
CSF_FUNC(int, __cdecl, ReleaseNativeHandle, 0x0081B9E0, (int* a1));
// Releases a RenderWare matrix (original 0x624D10).
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
// Game heap free (original 0x691110).
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
// Base-class constructor (original 0x6B2830).
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));

// Destroys a C_00861a60-derived immediate-mode object. Bit 0 of freeSelfFlag
// requests that the object's memory be released through the game heap.
static int __fastcall C_00861a78_Destroy(C_00861a78* self, void* /*edx*/, char freeSelfFlag)
{
    std::uint32_t handle = self->field_0004;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861A78);
    if (handle) {
        ReleaseNativeHandle(reinterpret_cast<int*>(handle));
        self->field_0004 = 0;
    }
    if (self->field_000c) {
        RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(self->field_0008));
    }
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861A60);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
    if ((freeSelfFlag & 1) != 0) {
        Mem_Free(self);
    }
    return reinterpret_cast<std::intptr_t>(self);
}

HOOK(0x6E3DD0, C_00861a78_Destroy, "auto");
