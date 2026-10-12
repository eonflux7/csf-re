#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Caches the object provider reached through the current scene's control into this+0x3C.
static int __fastcall GuiControl_CacheProviderAt3C(void* self, void* /*edx*/)
{
    uint32_t sceneFieldValue = CGuiControl_GetField012C(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);
    void* provider = GetCachedObjectProvider(
        reinterpret_cast<void*>(static_cast<uintptr_t>(sceneFieldValue)), nullptr);
    CSF_AT(void*, self, 0x3C) = provider;
    return static_cast<int>(reinterpret_cast<uintptr_t>(provider));
}

HOOK(0x004E08F0, GuiControl_CacheProviderAt3C, "auto");
