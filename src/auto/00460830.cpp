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

CSF_FUNC(int, __fastcall, GlobalSubObjectProcess_443000, 0x00443000, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(void*, __fastcall, GetGlobalSubObject_4FC0A0, 0x004FC0A0, (void* self, void* edx));

// Reads a float from the second argument block entry, truncates it to an integer, and if the
// truncated value is non-zero, forwards a small request record to the global sub-object processor.
static int __stdcall DispatchTruncatedArgIfNonZero(int unusedArg1, int unusedArg2, int* argBlock)
{
    (void)unusedArg1;
    (void)unusedArg2;

    // Request record passed by address to GlobalSubObjectProcess_443000.
    int requestRecord[4];
    requestRecord[0] = 2;
    requestRecord[1] = 0;
    requestRecord[2] = 0;
    requestRecord[3] = 0;

    uint32_t firstObjAddr = static_cast<uint32_t>(argBlock[0]);
    uint32_t firstSubObjAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(firstObjAddr), 4);
    requestRecord[1] = static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<void*>(firstSubObjAddr), 4));

    uint32_t secondObjAddr = static_cast<uint32_t>(argBlock[1]);
    uint32_t secondSubObjAddr = CSF_AT(uint32_t, reinterpret_cast<void*>(secondObjAddr), 4);
    float rawValue = CSF_AT(float, reinterpret_cast<void*>(secondSubObjAddr), 4);

    int64_t truncatedValue = static_cast<int64_t>(rawValue);
    uint32_t truncatedLow = static_cast<uint32_t>(truncatedValue);

    if (truncatedLow != 0)
    {
        requestRecord[3] = static_cast<int>(truncatedLow);

        C_00867260* globalValue = reinterpret_cast<C_00867260*>(
            static_cast<uintptr_t>(GetGlobalValue_4FBDE0()));
        void* globalSubObject = GetGlobalSubObject_4FC0A0(globalValue, nullptr);
        return GlobalSubObjectProcess_443000(
            globalSubObject,
            nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(&requestRecord[0])));
    }

    return 0;
}

HOOK(0x00460830, DispatchTruncatedArgIfNonZero, "auto");
