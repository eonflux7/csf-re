#include <cstdint>
#include "globals.h"
#include "hook.h"
CSF_GLOBAL(float, Scale, 0x0085798C);
static double __cdecl SharedHelper(float value) { return static_cast<double>(value) * Scale; }
HOOK(0x007A2AD0, SharedHelper, "stl");
