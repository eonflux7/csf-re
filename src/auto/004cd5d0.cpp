#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(unsigned int, __fastcall, C_0083ab58_SweepTargets, 0x004CD570, (void* self, void* edx));

// Target 0x4CD5D0 (__thiscall, ret 0). Calls SceneLoader_NoOp, then tail-jumps
// into 0x4CD570 with the same `this`.
static unsigned int __fastcall C_0083ab58_RunSearchSweep(void* self, void* /*edx*/)
{
    SceneLoader_NoOp();
    return C_0083ab58_SweepTargets(self, nullptr);
}

HOOK(0x004CD5D0, C_0083ab58_RunSearchSweep, "auto");
