#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores the flag argument at +0x1C. On the first call (latch byte at +0 still zero),
// records two option bytes at +0x15/+0x16, dispatches virtual slot 14 on the object at +4,
// and latches whether that dispatch returned -1 into the byte at +0. Returns the result.
static int __cdecl StoreFlagAndLatchDispatchResult(void* self, int a2, int a3, char a4, char a5)
{
    int result = a3;
    CSF_AT(int, self, 0x1C) = a3;

    if (CSF_AT(uint8_t, self, 0) == 0)
    {
        CSF_AT(char, self, 0x15) = a4;
        void* target = CSF_AT(void*, self, 0x04);
        CSF_AT(char, self, 0x16) = a5;

        int dispatchResult = ((int(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, target, 0)[0x38 / 4])(target, nullptr, a2, self);

        result = (dispatchResult == -1) ? 1 : 0;
        CSF_AT(uint8_t, self, 0) = (uint8_t)result;
    }
    return result;
}

HOOK(0x408A90, StoreFlagAndLatchDispatchResult, "auto");
