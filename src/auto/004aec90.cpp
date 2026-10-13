#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static void __fastcall AiFncTasks_AdvanceCharacterTimers(
    float* self, void* /*edx*/, float elapsedTime)
{
    if (CSF_AT(float, self, 0x260) > g_fZero)
        CSF_AT(float, self, 0x260) -= elapsedTime;

    if (CSF_AT(float, self, 0x2AC) > g_fZero)
        CSF_AT(float, self, 0x2AC) -= elapsedTime;

    if (CSF_AT(float, self, 0x2DC) > g_fZero)
        CSF_AT(float, self, 0x2DC) -= elapsedTime;

    if (CSF_AT(float, self, 0x2FC) > g_fZero)
        CSF_AT(float, self, 0x2FC) -= elapsedTime;

    if (CSF_AT(float, self, 0x318) > g_fZero)
        CSF_AT(float, self, 0x318) -= elapsedTime;

    if (CSF_AT(float, self, 0x334) > g_fZero)
        CSF_AT(float, self, 0x334) -= elapsedTime;

    if (CSF_AT(float, self, 0x36C) > g_fZero)
        CSF_AT(float, self, 0x36C) -= elapsedTime;

    if (CSF_AT(float, self, 0x390) > g_fZero)
        CSF_AT(float, self, 0x390) -= elapsedTime;

    if (CSF_AT(float, self, 0x3BC) > g_fZero)
        CSF_AT(float, self, 0x3BC) -= elapsedTime;
}

HOOK(0x004AEC90, AiFncTasks_AdvanceCharacterTimers, "auto");
