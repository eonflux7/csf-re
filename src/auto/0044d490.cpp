#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// Calls the object's slot-0 virtual for each entry of the list at entries[0] (count in entries[2]),
// unless the global state flag at +0xD0 is set. Returns the last slot-0 call's result, or the
// global state pointer / entries pointer as the original does.
static int __fastcall C_008311e8_ForEachSlot0Call(C_008311e8* self, void* /*edx*/, uint32_t* entries)
{
    int result = GetGlobalState_50D190();
    void* globalState = reinterpret_cast<void*>(static_cast<uintptr_t>(result));

    if (CSF_AT(uint8_t, globalState, 0xD0) == 0) {
        result = static_cast<int>(reinterpret_cast<uintptr_t>(entries));
        const uint32_t count = entries[2];
        for (uint32_t i = 0; i < count; ++i) {
            // The original reloads the list pointer every iteration.
            uint32_t* list = reinterpret_cast<uint32_t*>(entries[0]);
            uint32_t* item = reinterpret_cast<uint32_t*>(list[i]);
            // Raw slot-0 call: the original keeps the callee's eax as the function result.
            auto slot0 = reinterpret_cast<int(__fastcall*)(C_008311e8*, void*, uint32_t*)>(
                CSF_AT(void**, self, 0)[0]);
            result = slot0(self, nullptr, item);
        }
    }
    return result;
}

HOOK(0x0044D490, C_008311e8_ForEachSlot0Call, "auto");
