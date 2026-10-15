#include <cstdint>
#include "globals.h"
#include "hook.h"
#include <cmath>
CSF_GLOBAL(float, Zero, 0x0082D4CC);
static char __fastcall SharedHelper(float* self, void*, float length) {
    double norm = std::sqrt(static_cast<double>(self[0])*self[0] + static_cast<double>(self[1])*self[1] + static_cast<double>(self[2])*self[2]);
    if (norm == Zero) return 0;
    double scale = static_cast<double>(length) / norm;
    self[0] = static_cast<float>(self[0]*scale); self[1] = static_cast<float>(self[1]*scale); self[2] = static_cast<float>(self[2]*scale); return 1;
}
HOOK(0x0068E360, SharedHelper, "stl");
