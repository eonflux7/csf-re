#include <cmath>
#include "globals.h"
#include "hook.h"
CSF_GLOBAL(double, angle, 0x008312B8);
CSF_GLOBAL(float, cosine, 0x008AFC94);
static void __cdecl InitializeCosine() { cosine = static_cast<float>(std::cos(angle)); }
HOOK(0x00828A90, InitializeCosine, "initfn");
