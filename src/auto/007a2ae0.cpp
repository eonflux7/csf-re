#include <cstdint>
#include "globals.h"
#include "hook.h"
CSF_GLOBAL(float, Scale, 0x00875B98);
static double __cdecl SharedHelper(float value) { return static_cast<double>(value) * Scale; }
HOOK(0x007A2AE0, SharedHelper, "stl");
