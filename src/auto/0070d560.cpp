#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, FreeBlock_690F70, 0x00690F70, (void* block));
CSF_FUNC(void, __cdecl, RxPipelineDestroy, 0x00633780, (uint32_t* a1));
CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));
CSF_FUNC(char, __fastcall, sub_70D3F0, 0x0070D3F0, (void* self, void* edx));

static void __fastcall S_006fdbb0_ReleaseResources(S_006fdbb0* self, void* /*edx*/)
{
    // Free the first owned block, if any.
    if (self->field_00c8)
        FreeBlock_690F70(reinterpret_cast<void*>(self->field_00c8));

    // Release the object at +0xD8 through its vtable slot 1 (deleting call with flag 1),
    // then clear the slot.
    uint32_t object = self->field_00d8;
    if (object) {
        void* obj = reinterpret_cast<void*>(object);
        auto release = reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, obj, 0)[1]);
        release(obj, nullptr, 1);
        self->field_00d8 = 0;
    }

    // Internal cleanup of the rest of the state.
    sub_70D3F0(self, nullptr);

    // Destroy the render pipeline, if any.
    if (self->field_00f0)
        RxPipelineDestroy(reinterpret_cast<uint32_t*>(self->field_00f0));

    // Destroy the texture, if any.
    uint32_t texture = self->field_00d0;
    if (texture)
        RwTextureDestroy(static_cast<int>(texture));
}

HOOK(0x70D560, S_006fdbb0_ReleaseResources, "auto");
