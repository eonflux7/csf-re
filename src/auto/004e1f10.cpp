#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneSelection_CycleEligibleObject, 0x00451000, (unsigned int* self, void* , char backward));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083e5e8_CheckSceneMenuContextAndCycle(C_0083e5e8* self, void* /*edx*/)
{
    self->field_0018 = 5;
    uint32_t menuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    if (menuContext == self->field_000c) {
        return SceneSelection_CycleEligibleObject(static_cast<unsigned int*>(g_pCurrentScene), nullptr, 0);
    }
    return static_cast<char>(menuContext);
}

HOOK(0x4E1F10, C_0083e5e8_CheckSceneMenuContextAndCycle, "auto");
