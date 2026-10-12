#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee bindings (by original address)
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4DE860, 0x004DE860, (void* self, void* edx, int a2));

// Data
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Reads entry a2 (16-byte records at this->+0x0C), then forwards it, with the
// current scene's GUI control field, to sub_4DE860. The entry value stays on the
// stack for the callee, which pops it (ret 4).
static int __fastcall EntryTable_ForwardToSceneControl(void* self, void* /*edx*/, int a2)
{
    void* entryBase = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0xC));
    int32_t entryValue = static_cast<int32_t>(CSF_AT(uint32_t, entryBase, static_cast<uint32_t>(a2) << 4));

    uint32_t sceneField = CGuiControl_GetField012C(g_pCurrentScene, nullptr);

    return sub_4DE860(reinterpret_cast<void*>(sceneField), nullptr, entryValue);
}

HOOK(0x004E08C0, EntryTable_ForwardToSceneControl, "auto");
