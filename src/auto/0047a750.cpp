#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(int, __cdecl, IsSubObjectEligible, 0x007610B0, (int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008319c0_ApplySceneDependentWeaponAction(
    C_008319c0* self, void* /*edx*/)
{
    int action = CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x130);
    int subObject = C_008319c0_GetSubObjectField74(self, nullptr);

    if (static_cast<uint8_t>(IsSubObjectEligible(subObject)))
    {
        void* scene = g_pCurrentScene;
        auto* ownerContext = reinterpret_cast<uint8_t*>(self->field_0004) + 28;
        uint32_t menuContext = GetSceneMenuContext(scene, nullptr);
        char* sceneContext = S_00492b20_GetPtr0x1B4(
            reinterpret_cast<void*>(menuContext), nullptr);
        (void)CSF_VTBL(0x00492B20);

        if (CSF_AT(uint32_t, ownerContext, 4) == CSF_AT(uint32_t, sceneContext, 4))
            action = CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x138);
    }

    return self->vSlot7C(action, 1000002u, 1, 1);
}

HOOK(0x0047A750, C_008319c0_ApplySceneDependentWeaponAction, "auto");
