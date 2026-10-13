#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_MotionRandomScale, 0x840540);

// Advances the 32-bit Park-Miller style seed stored at +0xC4 and returns it scaled to [0,1) times timeScale.
static double __fastcall SceneMissionSetup_EvaluateAnimationTime(void* self, void* /*edx*/, float timeScale)
{
    const int32_t seed = CSF_AT(int32_t, self, 0xC4);
    const int32_t quotient = seed / 44488;
    const int32_t remainder = seed % 44488;
    const int32_t subtractTerm = 3399 * quotient;
    int32_t nextSeed = 48271 * remainder - subtractTerm;
    CSF_AT(int32_t, self, 0xC4) = nextSeed;
    if (nextSeed <= 0) {
        nextSeed = static_cast<int32_t>(static_cast<uint32_t>(nextSeed) + 0x7FFFFFFFu);
        CSF_AT(int32_t, self, 0xC4) = nextSeed;
    }
    return static_cast<double>(CSF_AT(int32_t, self, 0xC4)) * g_MotionRandomScale * timeScale;
}

HOOK(0x0050DD80, SceneMissionSetup_EvaluateAnimationTime, "auto");
