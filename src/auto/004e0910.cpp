#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __thiscall bool(std::uint32_t* this): compares the dword at this+0x3C with the
// cached object provider of the current scene's GuiControl field 0x12C.
static bool __fastcall IsFieldMatchingCurrentSceneProvider(void* self, void* /*edx*/)
{
    uint32_t sceneField = CGuiControl_GetField012C(g_pCurrentScene, nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(sceneField), nullptr);
    return CSF_AT(uint32_t, self, 0x3C) == reinterpret_cast<uint32_t>(provider);
}

HOOK(0x004E0910, IsFieldMatchingCurrentSceneProvider, "auto");
