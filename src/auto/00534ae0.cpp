#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(int, __fastcall, MenuSystem_ToggleChildFlag, 0x0050D300, (void* self, void* edx));
CSF_FUNC(uint32_t, __cdecl, GetGlobalValue, 0x00521BB0, ());
CSF_FUNC(int, __fastcall, FlushGlobalValueState, 0x00521F80, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ApplyGlobalValueSettings, 0x005233D0, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(uint32_t, __cdecl, GetSceneGlobalHandle, 0x0042D6D0, ());
CSF_FUNC(int, __fastcall, ApplyPauseSceneOption, 0x0042DF80, (void* self, void* edx, int option));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall CMenuMpPause_OnActivate(CMenuMpPause* self, void* /*edx*/, int a2)
{
    void* menuState = reinterpret_cast<void*>(GetGlobalState());
    MenuSystem_ToggleChildFlag(menuState, nullptr);

    void* owner = reinterpret_cast<void*>(self->field_0010);
    void** ownerVtbl = *reinterpret_cast<void***>(owner);
    reinterpret_cast<void(__fastcall*)(void*, void*, CMenuMpPause*, int)>(ownerVtbl[24])(owner, nullptr, self, 1);

    void* globalStateSecond = reinterpret_cast<void*>(GetGlobalState());
    uint32_t childObject = CSF_AT(uint32_t, globalStateSecond, 0x10);
    if (childObject != 0) {
        void* child = reinterpret_cast<void*>(childObject);
        void** childVtbl = *reinterpret_cast<void***>(child);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(childVtbl[9])(child, nullptr);
    }

    uint32_t sceneContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (sceneContext != 0) {
        uint32_t sceneInner = CSF_AT(uint32_t, reinterpret_cast<void*>(sceneContext), 0x18);
        if (sceneInner != 0) {
            uint8_t sceneFlag = *reinterpret_cast<uint8_t*>(sceneInner - 0x26D);
            if (sceneFlag != 0) {
                uint32_t handle = GetSceneGlobalHandle();
                ApplyPauseSceneOption(reinterpret_cast<void*>(handle), nullptr, a2);
            }
        }
    }

    if (self->field_0044) {
        uint32_t firstValue = GetGlobalValue();
        FlushGlobalValueState(reinterpret_cast<void*>(firstValue), nullptr);
        uint32_t secondValue = GetGlobalValue();
        ApplyGlobalValueSettings(reinterpret_cast<void*>(secondValue), nullptr);
    }
}

HOOK(0x00534AE0, CMenuMpPause_OnActivate, "auto");
