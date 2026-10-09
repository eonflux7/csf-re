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
CSF_FUNC(char, __fastcall, ComputeSlotStatusByte, 0x004EF150, (void* self, void* edx, void* a1, int a2));

static char __stdcall UpdateSlotStatusByte(int a1, int a2, uint32_t* a3)
{
    (void)a2;

    // Entry block: *a3 -> +4 -> +4 gives the slot entry pointer.
    uint8_t* ownerPtr = reinterpret_cast<uint8_t*>(a3[0]);
    uint8_t* slotEntry = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, ownerPtr, 4) + 4);

    // Context object; its field at +0x1FDC is loaded unconditionally and later used as the fastcall 'self'.
    uint8_t* context = reinterpret_cast<uint8_t*>(static_cast<intptr_t>(GetValue8B0554()));
    uint32_t contextField = CSF_AT(uint32_t, context, 0x1FDC);

    int32_t slotStatus = CSF_AT(int32_t, slotEntry, 8);

    if (slotStatus < 0)
    {
        uint8_t* stateBlock = reinterpret_cast<uint8_t*>(static_cast<intptr_t>(a1));
        uint8_t* stateFlags = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, stateBlock, 4));
        CSF_AT(uint8_t, stateFlags, 1) = 0;
        return static_cast<char>(slotStatus);
    }

    // Resolve the secondary chain: a3[1] -> +4 -> +4.
    uint8_t* secondary = reinterpret_cast<uint8_t*>(a3[1]);
    uint8_t* secondaryMid = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, secondary, 4));
    void* secondaryTarget = reinterpret_cast<void*>(CSF_AT(uint32_t, secondaryMid, 4));

    // Destination flag holder, loaded before the call.
    uint8_t* stateBlock = reinterpret_cast<uint8_t*>(static_cast<intptr_t>(a1));
    uint8_t* stateFlags = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, stateBlock, 4));

    char result = ComputeSlotStatusByte(
        reinterpret_cast<void*>(static_cast<intptr_t>(contextField)),
        reinterpret_cast<void*>(static_cast<intptr_t>(a1)),
        secondaryTarget,
        static_cast<int>(reinterpret_cast<uintptr_t>(slotEntry)));

    CSF_AT(uint8_t, stateFlags, 1) = static_cast<uint8_t>(result);
    return result;
}

HOOK(0x0045D000, UpdateSlotStatusByte, "auto");
