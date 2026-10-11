#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// The original's vtable reference (S_004a00e0). Not written by this function.
[[maybe_unused]] static const auto s_sceneRootVtbl = CSF_VTBL(0x004A00E0);

CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, IsFieldAt10Equal36, 0x00483360, (void* self, void*));
CSF_FUNC(void*, __fastcall, S_004a00e0_ShowAlertIcons, 0x004A00E0, (void* self, void* edx, void* arg0));
CSF_FUNC(void, __fastcall, S_004a00e0_ShowPlainIcons, 0x004A0330, (void* self, void* edx, void* arg0));

static void __fastcall S_004a00e0_RefreshAlertIcons(S_004a00e0* self, void* /*edx*/)
{
    if (!self->field_0005)
        return;

    int globalValue = GetGlobalValue_4FBDE0();
    int firstElement = GetFirstElement(reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue)), nullptr);
    void* cachedProvider = GetCachedObjectProvider(
        reinterpret_cast<void*>(static_cast<uintptr_t>(firstElement)), nullptr);
    GetReferencedObject(cachedProvider, nullptr);

    for (uint32_t i = 0; i < self->field_0028; ++i) {
        uint32_t element = *reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(self->field_0020) + 4 * i);
        uint32_t childObject = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(element)), 0x0C);
        if (childObject) {
            void* object = reinterpret_cast<void*>(static_cast<uintptr_t>(childObject));
            ((void(__fastcall*)(void*, void*))CSF_AT(void**, object, 0)[2])(object, nullptr);
        }
    }

    uint32_t sceneMenuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (sceneMenuContext) {
        void* menuContext = reinterpret_cast<void*>(static_cast<uintptr_t>(sceneMenuContext));
        void* activeFsm = CSF_AT(void*, menuContext, 0x44);
        if (IsFieldAt10Equal36(activeFsm, nullptr))
            S_004a00e0_ShowAlertIcons(self, nullptr, menuContext);
        else
            S_004a00e0_ShowPlainIcons(self, nullptr, menuContext);
    }
}

HOOK(0x004A0410, S_004a00e0_RefreshAlertIcons, "auto");
