#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwCameraEndUpdate, 0x00628E40, (void* camera));
CSF_FUNC(int, __cdecl, RwCameraBeginUpdate, 0x00628E50, (void* camera));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, MenuControls_ResolveObject, 0x006B1390, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, GfxSkin_ResetSkinState, 0x006C59E0, (void* self, void*));
CSF_FUNC(void, __fastcall, ClearFlagF4, 0x006C5A30, (uint8_t* self, void*));

static __declspec(noinline) void CGuiCharacterPreview_SetLightEnabled(
    CGuiCharacterPreview* self, int enabled)
{
    void* light = reinterpret_cast<void*>(self->m_pLight);
    ((void(__fastcall*)(void*, void*, int))
         CSF_AT(void**, light, 0)[0x2C / 4])(light, nullptr, enabled);
}

static int __fastcall CGuiCharacterPreview_Render(
    CGuiCharacterPreview* self, void* /*edx*/)
{
    if (!self->m_pScene)
        return 0;

    void* controls = MenuControls_ResolveObject(
        reinterpret_cast<void*>(GetGlobalInt()), nullptr);
    void* camera = reinterpret_cast<void*>(
        CSF_AT(uint32_t, controls, 0x0C));
    RwCameraEndUpdate(camera);

    void* manager = reinterpret_cast<void*>(
        ResolveClumpManager(GetGlobalInt(), nullptr));
    void* stateObject =
        ((void*(__fastcall*)(void*, void*))
             CSF_AT(void**, manager, 0)[0x48 / 4])(manager, nullptr);
    uint32_t* state =
        ((uint32_t*(__fastcall*)(void*, void*))
             CSF_AT(void**, stateObject, 0)[0x38 / 4])(stateObject, nullptr);

    alignas(8) uint8_t savedStateBytes[0x10];
    uint32_t* savedState = reinterpret_cast<uint32_t*>(savedStateBytes);
    savedState[0] = state[0];
    savedState[1] = state[1];
    savedState[2] = state[2];
    savedState[3] = state[3];

    manager = reinterpret_cast<void*>(
        ResolveClumpManager(GetGlobalInt(), nullptr));
    ((void(__fastcall*)(void*, void*, void*))
         CSF_AT(void**, manager, 0)[0x18 / 4])(
        manager, nullptr, reinterpret_cast<void*>(CSF_ADDR_DATA(0x0089A404)));

    CGuiCharacterPreview_SetLightEnabled(self, 1);

    void* character = reinterpret_cast<void*>(self->m_pCharacter);
    if (character)
    {
        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, character, 0)[0x6C / 4])(character, nullptr, 1);
    }

    GfxSkin_ResetSkinState(
        reinterpret_cast<void*>(GetGlobalValue_6C5760()), nullptr);

    void* scene = reinterpret_cast<void*>(self->m_pScene);
    ((void(__fastcall*)(void*, void*))
         CSF_AT(void**, scene, 0)[0x68 / 4])(scene, nullptr);

    ClearFlagF4(
        reinterpret_cast<uint8_t*>(GetGlobalValue_6C5760()), nullptr);

    character = reinterpret_cast<void*>(self->m_pCharacter);
    if (character)
    {
        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, character, 0)[0x6C / 4])(character, nullptr, 0);
    }

    CGuiCharacterPreview_SetLightEnabled(self, 0);

    manager = reinterpret_cast<void*>(
        ResolveClumpManager(GetGlobalInt(), nullptr));
    ((void(__fastcall*)(void*, void*, void*))
         CSF_AT(void**, manager, 0)[0x18 / 4])(manager, nullptr, savedState);

    return RwCameraBeginUpdate(camera);
}

HOOK(0x00541F60, CGuiCharacterPreview_Render, "auto");
