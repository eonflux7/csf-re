#include <cstdint>
#include "globals.h"
#include "hook.h"
CSF_FUNC(double, __cdecl, Wrap, 0x007A2990, (float value, float low, float high));
// Euler storage is y,x,z; choose equivalent angles nearest the current values.
static void __fastcall SharedHelper(float* self, void*, const float* target) {
    float old = self[1];
    double delta = Wrap(static_cast<float>(static_cast<double>(target[0]) - old), -3.1415927f, 3.1415927f);
    self[1] = static_cast<float>(delta + old);
    old = self[0];
    delta = Wrap(static_cast<float>(static_cast<double>(target[1]) - old), -3.1415927f, 3.1415927f);
    self[0] = static_cast<float>(delta + old);
    old = self[2];
    delta = Wrap(static_cast<float>(static_cast<double>(target[2]) - old), -3.1415927f, 3.1415927f);
    self[2] = static_cast<float>(delta + old);
}
HOOK(0x007A2A40, SharedHelper, "stl");
