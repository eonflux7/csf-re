#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void, __fastcall, S_004337e0_SetMenuSelection, 0x004337E0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_484380, 0x00484380, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));

// Vtable referenced by the S_004337e0 call sites; no store is emitted by the original.
[[maybe_unused]] static const auto kSceneSelectorVtbl = CSF_VTBL(0x004337E0);

static void __fastcall S_004337e0_SyncMenuState(S_004337e0* self, void* /*edx*/)
{
    uintptr_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    uint32_t menuIndexField = 0xFFFFFFFFu;

    if (sceneMenuContext != 0) {
        uint32_t stateCode = CSF_AT(uint32_t, reinterpret_cast<void*>(sceneMenuContext), 0x44);
        int menuMode = sub_484380(reinterpret_cast<void*>(static_cast<uintptr_t>(stateCode)), nullptr);

        switch (menuMode) {
        case 0:
            S_004337e0_SetMenuSelection(self, nullptr, 0u);
            break;
        case 1:
            S_004337e0_SetMenuSelection(self, nullptr, 1u);
            break;
        case 2:
            S_004337e0_SetMenuSelection(self, nullptr, 2u);
            break;
        case 3:
            S_004337e0_SetMenuSelection(self, nullptr, 3u);
            break;
        default: {
            int globalValue = GetGlobalValue_4FBDE0();
            int firstElement = GetFirstElement(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
            void* cachedProvider = GetCachedObjectProvider(
                reinterpret_cast<void*>(static_cast<uintptr_t>(firstElement)), nullptr);
            menuIndexField = CGuiControl_GetField012C(reinterpret_cast<CGuiControl*>(cachedProvider), nullptr);
            S_004337e0_SetMenuSelection(self, nullptr, menuIndexField);
            break;
        }
        }
        return;
    }

    S_004337e0_SetMenuSelection(self, nullptr, menuIndexField);
}

HOOK(0x00433870, S_004337e0_SyncMenuState, "auto");
