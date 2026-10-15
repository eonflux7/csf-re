#include <cstdint>
#include "globals.h"
#include "hook.h"
static double __fastcall SharedHelper(const float* self, void*) {
    return static_cast<double>(self[0])*self[0] + static_cast<double>(self[1])*self[1] + static_cast<double>(self[2])*self[2];
}
HOOK(0x0068E300, SharedHelper, "stl");
