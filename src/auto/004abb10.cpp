#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(int, g_AiSearchCostThisFrame, 0x8B044C);

// Ticks down a countdown stored at +0x10 (reset value at +0x0C) when the state at +0x4 is 1
// and the threshold at +0x8 is <= 0. When the countdown goes below zero it is reloaded and
// the object's virtual slot 33 (+0x84) is invoked with a2.
static void __fastcall AiMoveTasks_TickCountdownAndNotify(void* self, void* /*edx*/, int a2, float a3)
{
  if (CSF_AT(int, self, 0x4) == 1 && CSF_AT(float, self, 0x8) <= g_fZero)
  {
    float remaining = CSF_AT(float, self, 0x10) - a3;
    CSF_AT(float, self, 0x10) = remaining;
    if (remaining < g_fZero)
    {
      CSF_AT(float, self, 0x10) = CSF_AT(float, self, 0x0C);
      g_AiSearchCostThisFrame = 0;
      ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[0x84 / 4])(self, nullptr, a2);
    }
  }
}

HOOK(0x004ABB10, AiMoveTasks_TickCountdownAndNotify, "auto");
