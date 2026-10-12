#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee at 0x006E1710 is __cdecl. Bound as given in the packet (two arguments).
CSF_FUNC(char, __cdecl, sub_6E1710, 0x006E1710, (int a1, int a2));

// 0x006E1A60: __stdcall thunk with ret 8. The original reads the incoming ECX
// (it pushes ecx as a callee-visible value), so this is written as __fastcall:
// ECX arrives as the first parameter and the two stack arguments are callee-cleaned.
static int __fastcall ParticleEmitters_ForwardPairArguments(
    void* context, void* /*edx*/, int* firstArgument, int* secondArgument)
{
    (void)context;
    return static_cast<int>(sub_6E1710(
        reinterpret_cast<int>(firstArgument),
        reinterpret_cast<int>(secondArgument)));
}

HOOK(0x006E1A60, ParticleEmitters_ForwardPairArguments, "auto");
