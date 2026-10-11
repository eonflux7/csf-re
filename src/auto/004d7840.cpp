#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Bound by address: the original tail-jumps here after `sub ecx, 4`.
CSF_FUNC(void*, __fastcall, AiAutonomous_TailTargetSub4A6F10, 0x004A6F10, (void* self, void* edx, char a2));

// The original reads ecx on entry, so this is written with ecx as `self`
// (stack arg a1 at [esp+4], pops 4 via ret 4 of the callee).
static void* __fastcall AiAutonomous_ForwardWithAdjustedSelf(void* self, void* /*edx*/, char a1)
{
    void* adjustedSelf = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(self) - 4);
    return AiAutonomous_TailTargetSub4A6F10(adjustedSelf, nullptr, a1);
}

HOOK(0x004D7840, AiAutonomous_ForwardWithAdjustedSelf, "auto");
