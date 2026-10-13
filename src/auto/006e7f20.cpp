#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwFrameForAllChildren, 0x006237C0, (int a1, int (__cdecl* a2)(int, int), int a3));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));

// Destroys a bone-hierarchy object: detaches its child frames, frees the two
// owned buffers, tears down the embedded state, then re-runs the base constructor.
static void __fastcall C_00861da8_Destroy(C_00861da8* self, void* /*edx*/)
{
    uint32_t* v2 = &self->field_002c;
    int frameRoot = static_cast<int>(CSF_AT(uint32_t, self, 0x018));

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861DD8);

    uint32_t* childCursor = &self->field_002c;
    RwFrameForAllChildren(
        frameRoot,
        reinterpret_cast<int (__cdecl*)(int, int)>(CSF_ADDR_FN(0x006E7EA0)),
        static_cast<int>(reinterpret_cast<uintptr_t>(&childCursor)));

    if (self->field_0008) {
        CoreMemory_Free(reinterpret_cast<void*>(self->field_0008));
        self->field_0008 = 0;
    }
    if (*v2) {
        CoreMemory_Free(reinterpret_cast<void*>(*v2));
        *v2 = 0;
    }

    GfxObject_DestroyEmbeddedState(reinterpret_cast<C_00860630*>(self->pad_000c), nullptr);

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00861DA8);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x6E7F20, C_00861da8_Destroy, "auto");
