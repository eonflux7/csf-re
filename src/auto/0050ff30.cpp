#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, SceneLoader_ResetSceneEntries, 0x0050DB70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_RefreshPendingState, 0x0050FB20, (void* self, void* edx));
CSF_FUNC(int, __fastcall, UpdateEntriesFromMatchingId, 0x0071AA00, (void* self, void*));
CSF_FUNC(int, __fastcall, InvokeTrailingEntryCallbacks_71AA80, 0x0071AA80, (void* self, void*));
CSF_FUNC(int, __fastcall, InvokeTrailingEntryCallbacks_71AAC0, 0x0071AAC0, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(int, __fastcall, GfxSkin_ResetSkinState, 0x006C59E0, (void* self, void*));
CSF_FUNC(void, __fastcall, ClearFlagF4, 0x006C5A30, (uint8_t* self, void*));
CSF_GLOBAL(const char*, g_pszMenu_P_Intro, 0x00899B2C);

// Reached from the original 0x50FF30 (ecx = this, no stack args, ret 0).
static int __fastcall SceneLoader_PrepareGlobalState(S_0050f2f0* self, void* /*edx*/)
{
    // Fast path: the scene object exists and both readiness probes pass.
    if (self->field_0014 != 0) {
        void* sceneObjProbe = reinterpret_cast<void*>(self->field_0014);
        int notReady = ((int(__fastcall*)(void*, void*))CSF_AT(void**, sceneObjProbe, 0)[26])(sceneObjProbe, nullptr);
        if (notReady == 0) {
            void* sceneObjCheck = reinterpret_cast<void*>(self->field_0014);
            int checkResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, sceneObjCheck, 0)[27])(sceneObjCheck, nullptr);
            if (checkResult == 0) {
                return checkResult;
            }
        }
    }

    // Slow path: rebuild the scene entries and refresh pending state.
    SceneLoader_ResetSceneEntries(self, nullptr);
    UpdateEntriesFromMatchingId(reinterpret_cast<void*>(self->field_0014), nullptr);
    InvokeTrailingEntryCallbacks_71AA80(reinterpret_cast<void*>(self->field_0014), nullptr);
    InvokeTrailingEntryCallbacks_71AAC0(reinterpret_cast<void*>(self->field_0014), nullptr);
    SceneLoader_RefreshPendingState(self, nullptr);

    uint32_t pendingState = CSF_AT(uint32_t, self, 0x78);
    if (pendingState == 0) {
        return 0;
    }

    void* sceneObj = reinterpret_cast<void*>(self->field_0014);
    void** sceneVtbl = CSF_AT(void**, sceneObj, 0);
    int stateResult = ((int(__fastcall*)(void*, void*, const char*, int))sceneVtbl[23])(
        sceneObj, nullptr, g_pszMenu_P_Intro, 1);
    int flagResult = ((int(__fastcall*)(void*, void*, int))sceneVtbl[29])(sceneObj, nullptr, stateResult);

    if ((flagResult & 0xFF) != 0) {
        return flagResult;
    }

    // Former tail target at 0x4FB940, inlined: this = pending-state object (ecx = [self+0x78]).
    void* pending = reinterpret_cast<void*>(pendingState);
    uint32_t innerObj = CSF_AT(uint32_t, pending, 4);
    if (innerObj == 0) {
        return 0;
    }

    int globalValue = GetGlobalValue_6C5760();
    GfxSkin_ResetSkinState(reinterpret_cast<void*>(globalValue), nullptr);

    void* inner = reinterpret_cast<void*>(innerObj);
    ((int(__fastcall*)(void*, void*))CSF_AT(void**, inner, 0)[26])(inner, nullptr);

    int globalValueAfter = GetGlobalValue_6C5760();
    ClearFlagF4(reinterpret_cast<uint8_t*>(globalValueAfter), nullptr);
    return 0;
}

HOOK(0x0050FF30, SceneLoader_PrepareGlobalState, "auto");
