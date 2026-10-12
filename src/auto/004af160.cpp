#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, CPlayerCharacter_GetAlternateSceneSelection, 0x004950B0, (CPlayerCharacter* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Scales a float by the global factor when the handle-referenced character is alive
// and its alternate scene selection is below the scene's limit; otherwise returns it unchanged.
static float __fastcall TEstatico_ScaleByGlobalIfHandleTargetValid(void* self, void* /*edx*/, float value)
{
    if (CSF_AT(int, self, 0x608) == 3 && CSF_AT(int, self, 0x618) >= 0)
    {
        int handle = static_cast<int>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(self) + 0x610));
        void* target = reinterpret_cast<void*>(
            static_cast<uintptr_t>(static_cast<uint32_t>(CHandleTable_Resolve(g_pCurrentScene, nullptr, handle))));

        if (target != nullptr && CSF_AT(int, target, 0x50) == 0)
        {
            int alternateSelection = CPlayerCharacter_GetAlternateSceneSelection(
                reinterpret_cast<CPlayerCharacter*>(target), nullptr);

            int sceneLimit = CSF_AT(int, CSF_AT(void*, self, 0x2C), 0x7C);
            if (alternateSelection < sceneLimit)
            {
                uintptr_t globalBase = static_cast<uint32_t>(GetGlobalValue_8BC1E0());
                float globalFactor = CSF_AT(float, reinterpret_cast<void*>(globalBase), 0xAA4);
                return value * globalFactor;
            }
        }
    }
    return value;
}

HOOK(0x4AF160, TEstatico_ScaleByGlobalIfHandleTargetValid, "auto");
