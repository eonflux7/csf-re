#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int32_t, __cdecl, GetGlobal8afc78, 0x0042D6D0, ());
CSF_FUNC(int, __stdcall, MenuManager_SetGameplayControllerState, 0x0042DF80, (int a1));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CScene_SetFocusEntry, 0x0044FCE0, (CScene* self, void* , int requestedEntry));
CSF_FUNC(int, __fastcall, CScene_AddObjectFlags, 0x00452800, (CScene* self, void* , int handleAddress, int flags));
CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));
CSF_FUNC(int, __fastcall, C_00833b88_SetOverlayActive, 0x00499F00, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, ObjectProvider_SetPendingRequest, 0x004F5EF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ObjectProvider_SetUpdateMode, 0x004F7810, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

namespace {
struct ObjectHandleRequest
{
    void** vtbl;
    int    id;
    int    serial;
};
}

// Leaves the "captured" state: re-registers the object in the scene with its category mask,
// restores scene focus, resets the object provider request and re-enables the gameplay controller.
static char __fastcall C_00833b88_ReleaseCapture(C_00833b88* self, void* /*edx*/)
{
    char result = CSF_AT(char, self, 0x3A4);
    if (result)
    {
        uint32_t categoryMask = self->field_0094;
        int handleAddress = (int)(uintptr_t)&self->field_001c;

        CScene_RemoveObjectFromCategories(reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                                          handleAddress, (int)categoryMask);

        void* handleOwner = &self->field_000c;
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, handleOwner, 0)[1])(handleOwner, nullptr);

        CScene_AddObjectFlags(reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                              handleAddress, (int)categoryMask);
        CScene_SetFocusEntry(reinterpret_cast<CScene*>(g_pCurrentScene), nullptr, (int)(uintptr_t)self);

        C_00833b88_SetOverlayActive(self, nullptr, 0);

        int manager = GetGlobalValue_4FBDE0();
        int firstElement = GetFirstElement((void*)(uintptr_t)manager, nullptr);
        void* provider = GetCachedObjectProvider((void*)(uintptr_t)firstElement, nullptr);
        if (provider)
        {
            ObjectHandleRequest emptyRequest;
            emptyRequest.vtbl = (void**)CSF_VTBL(0x0082D448);
            emptyRequest.id = -1;
            emptyRequest.serial = -1;
            ObjectProvider_SetPendingRequest(provider, nullptr, (int)(uintptr_t)&emptyRequest);

            uint32_t menuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
            ObjectProvider_SetPendingRequest(provider, nullptr, (int)(menuContext + 0x1C));
            ObjectProvider_SetUpdateMode(provider, nullptr, 0);

            void* view = (void*)(uintptr_t)self->vGetField016c();
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, view, 0)[0x3C / 4])(view, nullptr, 1);

            view = (void*)(uintptr_t)self->vGetField016c();
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, view, 0)[0x48 / 4])(view, nullptr, 1);
        }

        GetGlobal8afc78();
        result = (char)MenuManager_SetGameplayControllerState(1);
        CSF_AT(char, self, 0x3A4) = 0;
    }
    return result;
}

HOOK(0x0049AA20, C_00833b88_ReleaseCapture, "auto");
