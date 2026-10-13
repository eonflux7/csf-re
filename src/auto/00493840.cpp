#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, C_00833908_IsActionEligible, 0x0048E860, (C_00833908* self, void*));

// Returns true when the action in self->field_0184 is enabled by the current scene's menu context.
// Actions whose mode (field_0050) is not 0x16 fall through to the base-class eligibility check.
static bool __fastcall C_00833cf8_IsSceneMenuActionEnabled(C_00833cf8* self, void* /*edx*/)
{
    if (self->field_0050 != 0x16)
    {
        return C_00833908_IsActionEligible(self, nullptr);
    }

    void* sceneMenuContext = reinterpret_cast<void*>(GetSceneMenuContext(g_pCurrentScene, nullptr));
    if (sceneMenuContext == nullptr)
    {
        return false;
    }

    uint32_t actionId = self->field_0184;
    if (actionId == 0xFFFFFFFFu)
    {
        return false;
    }

    // Virtual slot 0xC0 / 4 = 48 on the scene menu context object.
    auto isEnabled = reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, sceneMenuContext, 0)[48]);
    return isEnabled(sceneMenuContext, nullptr, static_cast<int>(actionId));
}

HOOK(0x00493840, C_00833cf8_IsSceneMenuActionEnabled, "auto");
