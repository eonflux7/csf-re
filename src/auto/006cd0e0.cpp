#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, GfxScene_PrepareWorldResources, 0x005655B0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, GfxScene_ActivateWorldResources, 0x006B6BC0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, ConsumeFlagByte26, 0x006B6BD0, (void* self, void*));
CSF_FUNC(void, __fastcall, GfxScene_ActivateRenderResources, 0x006BC080, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_ResetRenderState, 0x006BC7F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_PrepareRenderResources, 0x006BD3D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ClearRenderResources, 0x006BDAE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxScene_RefreshSceneController, 0x006CCB70, (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxScene_SetControllerState, 0x006D5D10, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, GfxScene_RefreshControllerState, 0x006D5E20, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_ClearWorldResources, 0x007512A0, (void* self, void* edx));

static char __fastcall C_00860bb8_ResetSceneController(
    C_00860bb8* self, void* /*edx*/)
{
    GfxScene_ResetRenderState(reinterpret_cast<void*>(self->field_0008), nullptr);
    ConsumeFlagByte26(reinterpret_cast<void*>(self->field_000c), nullptr);
    SceneLoader_ClearRenderResources(reinterpret_cast<void*>(self->field_0008), nullptr);
    SceneLoader_ClearWorldResources(reinterpret_cast<void*>(self->field_000c), nullptr);

    void* renderResources = reinterpret_cast<void*>(self->field_0008);
    if (renderResources)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, renderResources, 0)[1])(renderResources, nullptr, 1);
        self->field_0008 = 0;
    }

    void* worldResources = reinterpret_cast<void*>(self->field_000c);
    if (worldResources)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, worldResources, 0)[1])(worldResources, nullptr, 1);
        self->field_000c = 0;
    }

    uint32_t nextWorldResources = self->field_009c;
    uint32_t nextRenderResources = self->field_0098;
    self->field_000c = nextWorldResources;
    self->field_0008 = nextRenderResources;
    self->field_0098 = 0;
    self->field_009c = 0;

    GfxScene_PrepareRenderResources(reinterpret_cast<void*>(nextRenderResources), nullptr);
    GfxScene_PrepareWorldResources(reinterpret_cast<void*>(self->field_000c), nullptr);
    GfxScene_RefreshSceneController(self, nullptr);
    GfxScene_ActivateRenderResources(reinterpret_cast<void*>(self->field_0008), nullptr);
    GfxScene_ActivateWorldResources(reinterpret_cast<void*>(self->field_000c), nullptr);
    GfxScene_SetControllerState(
        reinterpret_cast<void*>(self->field_00bc), nullptr,
        static_cast<int>(self->field_00c0));
    GfxScene_RefreshControllerState(reinterpret_cast<void*>(self->field_00bc), nullptr);
    return 1;
}

HOOK(0x006CD0E0, C_00860bb8_ResetSceneController, "auto");
