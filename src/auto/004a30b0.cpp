#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

// Returns the animation scale stored at +0x30 of the selected entry, or the
// fallback slot's virtual value, or 0.0 when neither index is set.
static double __fastcall C_008344f0_GetAnimationValue(C_008344f0* self, void* /*edx*/)
{
    if (self->field_0048 == 0xFFFFFFFFu)
    {
        if (self->field_0044 != 0xFFFFFFFFu)
            return self->vSlot44(static_cast<int>(self->field_0044));
        return static_cast<double>(g_fZero);
    }

    uint8_t* controller = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
    uint32_t sceneObject = CSF_AT(uint32_t, controller, 0x1FC4);
    int entry = SceneMissionSetup_GetAnimationEntry(
        reinterpret_cast<void*>(static_cast<uintptr_t>(sceneObject)),
        nullptr,
        static_cast<int>(self->field_0048));
    uint8_t* entryPtr = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entry));
    return static_cast<double>(CSF_AT(float, entryPtr, 0x30));
}

HOOK(0x4A30B0, C_008344f0_GetAnimationValue, "auto");
