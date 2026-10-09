#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00833ee8_FireRepeatAction, 0x00497FB0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_fRepeatDelay, 0x00833E2C); // 0.21f
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

// Runs only while this object is the current scene's menu context: clears the "pressed" flag,
// resets the selection state if no valid selection, and fires the repeat action once the
// accumulated timer reaches the repeat delay (no delay when the global state's flag is set).
static void __fastcall C_00833ee8_UpdateRepeatTimer(C_00833ee8* self, void* /*edx*/)
{
    if (reinterpret_cast<uint32_t>(self) != GetSceneMenuContext(g_pCurrentScene, nullptr))
        return;

    int currentSelection = static_cast<int>(self->field_0278);
    self->field_0268 = 0;
    if (currentSelection < 0)
    {
        self->field_0268 = 0;
        self->field_026c = 0;
        self->field_0274 = 0xFFFFFFFF;
        self->field_0278 = 0xFFFFFFFF;
        self->field_027c = 0xFFFFFFFF;
        CSF_AT(float, self, 0x280) = g_ZeroVectorX;
        CSF_AT(float, self, 0x284) = g_ZeroVectorY;
        CSF_AT(float, self, 0x288) = g_ZeroVectorZ;
    }

    void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
    float threshold;
    if (CSF_AT(uint8_t, globalState, 0xD0) != 0)
        threshold = g_fZero;
    else
        threshold = g_fRepeatDelay;

    if (!(threshold > self->field_0210))
    {
        self->field_0210 = 0.0f;
        C_00833ee8_FireRepeatAction(self, nullptr);
    }
}

HOOK(0x00498310, C_00833ee8_UpdateRepeatTimer, "auto");
