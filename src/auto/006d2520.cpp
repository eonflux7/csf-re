#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00860170_Construct, 0x006B2830, (C_00860170* self, void*));
CSF_FUNC(int, __fastcall, C_00860e68_ReleaseLight, 0x006D1F30, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, GfxLights_ReleaseOwnedState, 0x006D24A0, (void* self, void* edx));

static void __fastcall C_00860e68_ReleaseBaseState(
    C_00860e68* self, void* /*edx*/)
{
    void* ownedState = reinterpret_cast<void*>(self->field_0008);

    self->vtbl = CSF_VTBL(0x00860E68);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x00860E64);

    // Listed by the packet as a vtable reference at the destructor call.
    (void)CSF_VTBL(0x006D24A0);

    if (ownedState)
    {
        GfxLights_ReleaseOwnedState(ownedState, nullptr);
        Mem_Free(ownedState);
        self->field_0008 = 0;
    }

    void* sceneState = reinterpret_cast<void*>(self->field_000c);
    if (sceneState)
    {
        SceneLoader_NoOp();
        Mem_Free(sceneState);
        self->field_000c = 0;
    }

    C_00860e68_ReleaseLight(self, nullptr, static_cast<int>(self->field_002c));
    C_00860e68_ReleaseLight(self, nullptr, static_cast<int>(self->field_0030));

    void* block = reinterpret_cast<void*>(self->field_0088);
    if (block)
    {
        Mem_Free(block);
        self->field_0088 = 0;
    }

    block = reinterpret_cast<void*>(self->field_0074);
    if (block)
    {
        Mem_Free(block);
        self->field_0074 = 0;
    }

    block = reinterpret_cast<void*>(self->field_0064);
    if (block)
    {
        Mem_Free(block);
        self->field_0064 = 0;
    }

    block = reinterpret_cast<void*>(self->field_0054);
    if (block)
    {
        Mem_Free(block);
        self->field_0054 = 0;
    }

    block = reinterpret_cast<void*>(self->field_0044);
    if (block)
    {
        Mem_Free(block);
        self->field_0044 = 0;
    }

    block = reinterpret_cast<void*>(self->field_0034);
    if (block)
    {
        Mem_Free(block);
        self->field_0034 = 0;
    }

    self->vtbl = CSF_VTBL(0x00860DE0);
    CSF_AT(void**, self, 0x04) = CSF_VTBL(0x00860DDC);
    C_00860170_Construct(reinterpret_cast<C_00860170*>(self), nullptr);
}

HOOK(0x006D2520, C_00860e68_ReleaseBaseState, "auto");
