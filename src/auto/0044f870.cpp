#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static double __fastcall CGuiControl_GetSceneScaleValue(CGuiControl* self, void* /*edx*/)
{
    float transitionOne = g_TransitionOne;
    float controlDivisor = *reinterpret_cast<float*>(&self->field_0124);
    float controlFactor = *reinterpret_cast<float*>(&self->field_0120);
    float scaledValue = transitionOne / controlDivisor;
    return (double)(scaledValue * controlFactor);
}

HOOK_KEEP_ECX(0x0044F870, CGuiControl_GetSceneScaleValue, 0, "game_scene_access");
