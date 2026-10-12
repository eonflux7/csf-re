#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, C_00830578_ResetMenuController, 0x0042CAB0, (C_00830578* self, void*));
CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __stdcall, MenuManager_SetGameplayControllerState, 0x0042DF80, (int a1));
CSF_FUNC(int, __fastcall, CScene_AddObjectFlags, 0x00452800, (CScene* self, void* , int handleAddress, int flags));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));
CSF_FUNC(int, __fastcall, CCharActionFsm_ResolveActionTarget_486E60, 0x00486E60, (CCharActionFsm* self, void*));
CSF_FUNC(char*, __fastcall, ObjectProvider_GetCurrentEntry, 0x004F4E90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall HudAlertIcons_PrepareCapture(void* self, void* /*edx*/)
{
    if (!CSF_AT(uint8_t, self, 0x3A4))
    {
        GetGlobal8afc78();
        MenuManager_SetGameplayControllerState(0);

        int collection = GetGlobalValue_4FBDE0();
        int firstElement = GetFirstElement(reinterpret_cast<void*>(collection), nullptr);
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(firstElement), nullptr);

        if (provider)
        {
            char* entry = ObjectProvider_GetCurrentEntry(provider, nullptr);
            if (CSF_AT(int32_t, entry, 0x08) >= 0)
            {
                int categoryMask = CSF_AT(int, self, 0x94);
                void* handle = static_cast<uint8_t*>(self) + 0x1C;

                CScene_RemoveObjectFromCategories(
                    reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                    reinterpret_cast<int>(handle), categoryMask);

                void* captureState = static_cast<uint8_t*>(self) + 0x0C;
                reinterpret_cast<void(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, captureState, 0)[1])(captureState, nullptr);

                CScene_AddObjectFlags(
                    reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                    reinterpret_cast<int>(handle), categoryMask);

                void* display = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, display, 0)[0x3C / 4])(display, nullptr, 0);

                display = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, self, 0)[0x28 / 4])(self, nullptr);
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, display, 0)[0x48 / 4])(display, nullptr, 0);

                int target = CCharActionFsm_ResolveActionTarget_486E60(
                    CSF_AT(CCharActionFsm*, self, 0x44), nullptr);
                if (target)
                {
                    void* actionTarget = reinterpret_cast<void*>(target);
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, actionTarget, 0)[0x30 / 4])(
                            actionTarget, nullptr, 0);
                }
            }
        }
    }

    CSF_AT(uint8_t, self, 0x3A4) = 1;
    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    return C_00830578_ResetMenuController(
        CSF_AT(C_00830578*, globalState, 0x18), nullptr);
}

HOOK(0x004957C0, HudAlertIcons_PrepareCapture, "auto");
