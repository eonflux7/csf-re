#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, StateRecord_Reset, 0x00401870, (void* self, void* edx, int a2));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(void, __fastcall, Hud_ClearPendingWidgetState, 0x0042E010, (void* self, void*));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __fastcall, HudUpdateSceneMenuVisibility, 0x0042FA50, (void* self, void*));
CSF_FUNC(int, __cdecl, FxParticleManager_HasNamedEffect, 0x00437D60, (unsigned int* a1, char* a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(int, __fastcall, MenuManager_InitEntry, 0x0050C860, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, MenuManager_SetIntroPageData, 0x006F2850, (void* self, void* edx, void* data));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszMenu_P_Intro, 0x00899B2C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall Interface_UpdateIntroMenu(
    void* self, void* /*edx*/, int /*unused*/)
{
    int result = GetGlobalValue_8B0568();
    void* state = reinterpret_cast<void*>(result);

    if (CSF_AT(int, state, 0x5C) == 3)
    {
        Hud_ClearPendingWidgetState(
            reinterpret_cast<void*>(GetGlobal8afc78()), nullptr);

        if (!CSF_AT(uint8_t, self, 0x108) &&
            !CSF_AT(uint8_t, self, 0x109))
        {
            void* currentState =
                reinterpret_cast<void*>(GetGlobalValue_8B0568());
            bool activateScene = CompareStringSubstring(
                static_cast<uint8_t*>(currentState) + 0x98,
                nullptr, 0, CSF_AT(uint32_t, currentState, 0xAC),
                reinterpret_cast<const char*>(&g_byte_82E65C), 0) != 0;

            if (!activateScene)
            {
                void* effectEntry =
                    reinterpret_cast<uint8_t*>(GetValue8B0554()) + 7920;

                if (static_cast<uint8_t>(FxParticleManager_HasNamedEffect(
                        reinterpret_cast<unsigned int*>(effectEntry),
                        reinterpret_cast<char*>(&g_byte_82E65C))) == 0 ||
                    CSF_AT(uint8_t, state, 0xE1) ||
                    CSF_AT(uint8_t,
                        reinterpret_cast<void*>(GetGlobalValue_75FDF0()),
                        0xCE))
                {
                    alignas(8) uint8_t resetRecordBytes[0x34];
                    alignas(8) uint8_t notificationRecordBytes[0x34];
                    C_0082d478& resetRecord =
                        *reinterpret_cast<C_0082d478*>(resetRecordBytes);
                    C_0082d478& notificationRecord =
                        *reinterpret_cast<C_0082d478*>(notificationRecordBytes);

                    if (CSF_AT(uint8_t,
                            reinterpret_cast<void*>(GetGlobalValue_75FDF0()),
                            0xCF))
                    {
                        uint32_t* record =
                            StateRecord_Reset(&notificationRecord, nullptr, 43);
                        void* dispatcher =
                            reinterpret_cast<void*>(GetGlobalObjectField184());
                        DispatchRecordToGlobalObject(
                            dispatcher, nullptr, record, 1);
                        notificationRecord.field_0014 =
                            reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                        notificationRecord.field_0008 =
                            reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                        notificationRecord.vtbl = CSF_VTBL(0x0082D410);
                    }

                    uint32_t* record =
                        StateRecord_Reset(&resetRecord, nullptr, 0);
                    void* dispatcher =
                        reinterpret_cast<void*>(GetGlobalObjectField184());
                    DispatchRecordToGlobalObject(
                        dispatcher, nullptr, record, 1);
                    resetRecord.field_0014 =
                        reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                    resetRecord.field_0008 =
                        reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                    resetRecord.vtbl = CSF_VTBL(0x0082D410);

                    GetGlobalState_50D190();
                    activateScene = true;
                }
                else
                {
                    void* entry = static_cast<uint8_t*>(self) + 0x100;
                    MenuManager_InitEntry(entry, nullptr);

                    void* effectName;
                    if (CSF_AT(uint32_t, effectEntry, 0x18) < 0x10u)
                        effectName = static_cast<uint8_t*>(effectEntry) + 4;
                    else
                        effectName = CSF_AT(void*, effectEntry, 0x04);

                    void* menu = CSF_AT(void*, self, 0x18);
                    CSF_AT(void*, self, 0x100) = effectName;
                    CSF_AT(uint32_t, self, 0x104) = 1;
                    CSF_AT(uint8_t, self, 0x110) = 0;
                    CSF_AT(uint8_t, self, 0x111) = 1;

                    void* page =
                        reinterpret_cast<void* (__fastcall*)(
                            void*, void*, const char*, void*)>(
                            CSF_AT(void**, menu, 0)[0x5C / 4])(
                                menu, nullptr, g_pszMenu_P_Intro, entry);

                    // The original supplies saved-register garbage here.
                    MenuManager_SetIntroPageData(page, nullptr, nullptr);

                    menu = CSF_AT(void*, self, 0x18);
                    reinterpret_cast<void (__fastcall*)(
                        void*, void*, const char*)>(
                        CSF_AT(void**, menu, 0)[0x58 / 4])(
                            menu, nullptr, g_pszMenu_P_Intro);
                }
            }

            if (activateScene)
            {
                void* menu = CSF_AT(void*, self, 0x18);
                CSF_AT(uint8_t, self, 0x108) = 1;
                CSF_AT(uint8_t, self, 0x17F) = 1;

                void* sceneState = CSF_AT(void*, g_pCurrentScene, 0xC0);
                uint32_t visibilityArgument =
                    (reinterpret_cast<uint32_t>(sceneState) & 0xFFFFFF00u) |
                    static_cast<uint32_t>(
                        CSF_AT(uint8_t, sceneState, 0x09) == 0);

                reinterpret_cast<void (__fastcall*)(
                    void*, void*, uint32_t)>(
                    CSF_AT(void**, menu, 0)[0x9C / 4])(
                        menu, nullptr, visibilityArgument);

                menu = CSF_AT(void*, self, 0x18);
                reinterpret_cast<void (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, menu, 0)[0xA8 / 4])(menu, nullptr);
            }
        }

        CSF_AT(uint8_t, self, 0x17D) = 0;
        HudUpdateSceneMenuVisibility(
            reinterpret_cast<void*>(GetGlobal8afc78()), nullptr);
        result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, self, 0)[0x98 / 4])(self, nullptr);
        CSF_AT(uint8_t, self, 0x17D) = 1;
    }

    CSF_AT(uint32_t, self, 0x134) = 0;
    return result;
}

HOOK(0x00537740, Interface_UpdateIntroMenu, "auto");
