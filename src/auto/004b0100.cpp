#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

// Static/guard AI: stores two args from a2 into the state block at +0x58C/+0x590
// and forwards the state block's +0x57C sub-object to CoverAttack_SetState.
static int __stdcall StaticAi_SetGuardStateFromArgs(C_0085e7b8* a1, int a2)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int firstBase = linkedValue ? linkedValue - 4 : 0;
    UnnamedApply_4DC180(a1, nullptr, firstBase + 0x574);

    int secondLinked = GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int stateBaseAddr = secondLinked ? secondLinked - 4 : 0;
    void* stateBase = reinterpret_cast<void*>(static_cast<uintptr_t>(stateBaseAddr));

    void* cachedProvider = GetCachedObjectProvider(a1, nullptr);

    void* argBlock = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));
    CSF_AT(int, stateBase, 0x58C) = CSF_AT(int, argBlock, 4);
    CSF_AT(int, stateBase, 0x590) = CSF_AT(int, argBlock, 8);

    return CoverAttack_SetState_4DC150(
        cachedProvider, nullptr,
        static_cast<int>(static_cast<uintptr_t>(stateBaseAddr) + 0x57C));
}

HOOK(0x4B0100, StaticAi_SetGuardStateFromArgs, "auto");
