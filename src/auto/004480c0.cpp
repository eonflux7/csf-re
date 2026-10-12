#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(double, __fastcall, MenuSystem_GetIndexedGlobalFloat, 0x0050DA90, (void* self, void*));

static void __fastcall SceneLoader_ResetSceneState(void* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x28) = 0;
    CSF_AT(std::uint32_t, self, 0x24) = 0;
    CSF_AT(std::uint32_t, self, 0x58) = 0;
    CSF_AT(float, self, 0x5C) = 0.2f;

    const int globalState = GetGlobalState_50D190();
    CSF_AT(float, self, 0x60) = static_cast<float>(
        MenuSystem_GetIndexedGlobalFloat(reinterpret_cast<void*>(globalState), nullptr));
}

HOOK(0x004480C0, SceneLoader_ResetSceneState, "auto");
