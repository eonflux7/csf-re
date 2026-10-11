#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

// Returns the float at +0x30 of the animation entry selected by the virtual slot 0x70 index,
// or 0.0 when that index is -1.
static double __fastcall C_008344f0_GetCurrentAnimationValue(C_008344f0* self, void* /*edx*/, int a2)
{
    int entryIndex = self->vSlot70(a2);
    if (entryIndex == -1)
        return 0.0;

    int sceneHandle = GetValue8B0554();
    uint8_t* scene = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(sceneHandle));
    void* missionSetup = CSF_AT(void*, scene, 0x1FC4);

    int entry = SceneMissionSetup_GetAnimationEntry(missionSetup, nullptr, entryIndex);
    float value = CSF_AT(float, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entry)), 0x30);
    return static_cast<double>(value);
}

HOOK(0x4A3130, C_008344f0_GetCurrentAnimationValue, "auto");
