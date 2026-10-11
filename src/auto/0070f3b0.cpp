#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

CSF_FUNC(int, __cdecl, SoundMax_SetChannelGain, 0x007F0E80, (int a1, float a2));
CSF_FUNC(void, __cdecl, SoundMax_ApplyMasterLevel, 0x00712770, (float a1, int a2, int a3));

// Clamps a level to [0, 1], then calls SoundMax_SetChannelGain(this->field_0004, 1.0)
// and SoundMax_ApplyMasterLevel(level). The original's stack slots for the last two
// arguments of ApplyMasterLevel are uninitialized, so they are passed as 0.
static void __fastcall C_00863ba8_SetClampedLevel(C_00863ba8* self, void* /*edx*/, float level)
{
  if (level <= g_TransitionOne) {
    if (level < g_fZero) {
      level = g_fZero;
    }
  } else {
    level = g_TransitionOne;
  }

  SoundMax_SetChannelGain((int)self->field_0004, g_TransitionOne);
  SoundMax_ApplyMasterLevel(level, 0, 0);
}

HOOK(0x0070F3B0, C_00863ba8_SetClampedLevel, "auto");
