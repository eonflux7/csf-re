#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_FUNC(uint32_t, __fastcall, CGuiControl_GetField012C, 0x0044F630, (CGuiControl* self, void*));
CSF_FUNC(bool, __cdecl, SceneSubObjectTest, 0x0041E380, (void* a1));

// Clears the state byte at ctrlState+1, then, if the current scene's control
// reports a nonzero field, stores the result of SceneSubObjectTest into it.
// Returns the low byte of that result (0 when the scene control field is zero).
static char __stdcall UpdateSceneControlFlag(void* a1, int /*a2*/, void* a3)
{
    CSF_AT(uint8_t*, a1, 4)[1] = 0;

    uint32_t sceneControlField = CGuiControl_GetField012C(
        reinterpret_cast<CGuiControl*>(g_pCurrentScene), nullptr);

    char result = static_cast<char>(sceneControlField);
    if (sceneControlField != 0)
    {
        uint32_t argBlock = CSF_AT(uint32_t, a3, 0);
        uint32_t subBlock = CSF_AT(uint32_t, argBlock, 4);
        bool ready = SceneSubObjectTest(reinterpret_cast<void*>(subBlock + 4));

        CSF_AT(uint8_t*, a1, 4)[1] = static_cast<uint8_t>(ready);
        result = static_cast<char>(ready);
    }
    return result;
}

HOOK(0x0045AEF0, UpdateSceneControlFlag, "auto");
