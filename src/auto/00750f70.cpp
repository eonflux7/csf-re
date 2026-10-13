#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Forwards to the NetPlayerDdl base-properties helper at 0x7A38E0. The original
// takes (a1, middle, a2) as stdcall, ignores ecx, and pushes the middle and a2
// arguments as the helper's two stack arguments, so no frame is needed.
CSF_FUNC(void, __fastcall, NetPlayerDdl_ApplyBaseProperties, 0x007A38E0, (void* self, void* edx, void* a2, int a3));

static void __stdcall NetPlayerDdl_ForwardBaseProperties(void* a1, int a3, void* a2)
{
    NetPlayerDdl_ApplyBaseProperties(a1, nullptr, a2, a3);
}

HOOK(0x00750F70, NetPlayerDdl_ForwardBaseProperties, "auto");
