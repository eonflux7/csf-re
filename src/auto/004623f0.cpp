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

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));

static unsigned int __stdcall DispatchResolvedTargetRequest(int a1, int unusedSecond, uint32_t* requestArgs)
{
    (void)a1;
    (void)unusedSecond;

    // v3 = *(*a3 + 4)
    const uint32_t firstNode = CSF_AT(uint32_t, reinterpret_cast<void*>(requestArgs[0]), 4);
    // v4 = *(v3 + 12)
    const int32_t v4 = static_cast<int32_t>(CSF_AT(uint32_t, reinterpret_cast<void*>(firstNode), 12));
    // result = v3 + 4
    uint32_t result = firstNode + 4;

    if (v4 >= 0)
    {
        uint32_t argBlock[7];
        argBlock[0] = 2;
        argBlock[1] = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
        // *(result + 4) == *(v3 + 8)
        argBlock[2] = CSF_AT(uint32_t, reinterpret_cast<void*>(firstNode), 8);
        argBlock[3] = static_cast<uint32_t>(v4);

        // B = *(a3[1] + 4); arguments are B's embedded sub-object fields at +4, +8, +12
        const uint32_t sourceOwner = CSF_AT(uint32_t, reinterpret_cast<void*>(requestArgs[1]), 4);
        argBlock[4] = CSF_AT(uint32_t, reinterpret_cast<void*>(sourceOwner), 4);
        argBlock[5] = CSF_AT(uint32_t, reinterpret_cast<void*>(sourceOwner), 8);
        argBlock[6] = CSF_AT(uint32_t, reinterpret_cast<void*>(sourceOwner), 12);

        const uint32_t lookupResult = static_cast<uint32_t>(GetValue8B0554());

        // mov ecx, [eax + 0x1fd8]; add ecx, 0x34
        const uint32_t dispatchBase = CSF_AT(uint32_t, reinterpret_cast<void*>(lookupResult), 0x1FD8);
        void* dispatchSelf = reinterpret_cast<void*>(dispatchBase + 0x34);

        return DispatchWithArgBlock(dispatchSelf, nullptr, argBlock);
    }

    return result;
}

HOOK(0x004623F0, DispatchResolvedTargetRequest, "auto");
