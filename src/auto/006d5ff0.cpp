#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0085e7b8_GetSceneRegionAt, 0x004FC130, (C_0085e7b8* self, void* , int index));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(C_00860178*, __fastcall, C_00860178_Construct, 0x006B2820, (C_00860178* self, void*));
CSF_FUNC(int, __fastcall, C_00860894_Init, 0x006C45F0, (C_00860894* self, void* , int a2, int a3));
CSF_FUNC(int, __fastcall, C_008670f0_SetField0C, 0x006C4620, (C_008670f0* self, void* , int value));
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, SceneLoader_ResetRenderMember, 0x006D5BB0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, GfxScene_RefreshControllerState, 0x006D5E20, (void* self, void* edx));

static char __fastcall C_0085e7b8_InitializeSceneAttachment(
    C_0085e7b8* self, void* /*edx*/, int sceneId)
{
    if (CSF_AT(std::uint8_t, self, 0x00))
    {
        auto* oldAttachment = CSF_AT(C_00860178*, self, 0x4C);
        if (oldAttachment)
        {
            oldAttachment->vSlot4(1);
            CSF_AT(C_00860178*, self, 0x4C) = nullptr;
        }
        SceneLoader_ResetRenderMember(self, nullptr);
    }

    CSF_AT(std::uint8_t, self, 0x00) = 0;
    CSF_AT(std::uint8_t, self, 0x01) = 1;
    CSF_AT(std::uint8_t, self, 0x34) = 0;
    CSF_AT(std::uint8_t, self, 0x48) = 0;
    CSF_AT(std::uint8_t, self, 0x49) = 0;
    CSF_AT(std::uint8_t, self, 0x8C) = 0;
    CSF_AT(int, self, 0x90) = 0;
    CSF_AT(int, self, 0x94) = 0;
    CSF_AT(int, self, 0x9C) = -8388609;
    CSF_AT(int, self, 0x44) = 1;
    CSF_AT(std::uint8_t, self, 0xA0) = 0;
    CSF_AT(int, self, 0x98) = sceneId;

    auto* attachment =
        reinterpret_cast<C_00860178*>(CoreMemory_AllocUntagged(28));
    if (attachment)
    {
        C_00860178_Construct(attachment, nullptr);
        attachment->vtbl = CSF_VTBL(0x00861080);
        C_00860894_Init(
            reinterpret_cast<C_00860894*>(
                reinterpret_cast<std::uint8_t*>(attachment) + 0x08),
            nullptr, -1, 0);
        CSF_AT(C_0085e7b8*, attachment, 0x04) = self;
    }
    CSF_AT(C_00860178*, self, 0x4C) = attachment;

    auto* globalState = reinterpret_cast<void*>(GetGlobalValue_6C5760());
    auto* scene = CSF_AT(C_0085e7b8*, globalState, 0x04);
    attachment = CSF_AT(C_00860178*, self, 0x4C);
    const int region = C_0085e7b8_GetSceneRegionAt(scene, nullptr, 7);
    auto* regionBinding =
        reinterpret_cast<C_008670f0* (__fastcall*)(void*, void*)>(
            attachment->vtbl[0x1C / 4])(attachment, nullptr);
    C_008670f0_SetField0C(regionBinding, nullptr, region);

    auto* firstState = reinterpret_cast<std::uint8_t*>(self) + 0x50;
    GfxSkin_SetKeyedValue(firstState, nullptr, 10, 1);
    GfxSkin_SetKeyedValue(firstState, nullptr, 11, 2);
    GfxSkin_SetKeyedValue(firstState, nullptr, 6, 1);
    GfxSkin_SetKeyedValue(firstState, nullptr, 8, 1);

    auto* secondState = reinterpret_cast<std::uint8_t*>(self) + 0x64;
    GfxSkin_SetKeyedValue(secondState, nullptr, 12, 0);
    GfxSkin_SetKeyedValue(secondState, nullptr, 8, 1);
    GfxSkin_SetKeyedValue(secondState, nullptr, 6, 1);
    GfxSkin_SetKeyedValue(secondState, nullptr, 20, 2);
    GfxSkin_SetKeyedValue(secondState, nullptr, 10, 2);
    GfxSkin_SetKeyedValue(secondState, nullptr, 11, 1);
    GfxSkin_SetKeyedValue(secondState, nullptr, 14, 1);

    auto* thirdState = reinterpret_cast<std::uint8_t*>(self) + 0x78;
    GfxSkin_SetKeyedValue(thirdState, nullptr, 12, 1);
    GfxSkin_SetKeyedValue(thirdState, nullptr, 8, 1);
    GfxSkin_SetKeyedValue(thirdState, nullptr, 6, 1);
    GfxSkin_SetKeyedValue(thirdState, nullptr, 20, 1);
    GfxSkin_SetKeyedValue(thirdState, nullptr, 10, 5);
    GfxSkin_SetKeyedValue(thirdState, nullptr, 11, 6);
    GfxSkin_SetKeyedValue(thirdState, nullptr, 14, 1);

    GfxScene_RefreshControllerState(self, nullptr);
    CSF_AT(std::uint8_t, self, 0x00) = 1;
    return 1;
}

HOOK(0x006D5FF0, C_0085e7b8_InitializeSceneAttachment, "auto");
