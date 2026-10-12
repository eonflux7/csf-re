#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Float constants referenced by the original.
CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(float, g_fEpsilon, 0x83F384);

CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(char, __fastcall, SoundManager_PlaySoundInSceneMenuContext, 0x00525AB0, (int* self, void* /*edx*/, int soundArg));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Plays the movement sound when the anim is in state 2 and the global timer
// has advanced past the stored start value.
static void __fastcall CommandoAnims_TryPlayMovementSound(uint8_t* self, void* /*edx*/)
{
    if (CSF_AT(float, self, 0x80) > g_fZero
        && CSF_AT(int, self, 0x7C) == 2)
    {
        uint8_t* globals = reinterpret_cast<uint8_t*>(GetGlobalValue_8BC1E0());
        float elapsed = CSF_AT(float, globals, 0x18B0) - CSF_AT(float, self, 0x80);
        if (elapsed > g_fEpsilon)
        {
            int* soundManager = reinterpret_cast<int*>(Get_008b0678());
            SoundManager_PlaySoundInSceneMenuContext(soundManager, nullptr, 60);
        }
    }
}

HOOK(0x004FC050, CommandoAnims_TryPlayMovementSound, "auto");
