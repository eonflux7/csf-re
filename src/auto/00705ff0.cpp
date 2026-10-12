#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original: int __cdecl sub_705FF0(int a1, int a2), ret 0.
// Fetches the global int, resolves the clump manager for it, then runs the
// per-object routine at 0x6D17B0 with a2 as its pointer argument and 0 as the flag.

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));
CSF_FUNC(char, __fastcall, sub_6D17B0, 0x006D17B0, (void* self, void* edx, unsigned int* arg0, int a3));

static int __cdecl ParticleRender_InvokeClumpRoutine(int /*a1*/, int a2)
{
    int globalInt = GetGlobalInt();
    int clumpManagerId = ResolveClumpManager(globalInt, nullptr);
    void* clumpManager = reinterpret_cast<void*>(static_cast<uintptr_t>(clumpManagerId));
    return static_cast<int>(sub_6D17B0(clumpManager, nullptr,
                                       reinterpret_cast<unsigned int*>(a2), 0));
}

HOOK(0x705FF0, ParticleRender_InvokeClumpRoutine, "auto");
