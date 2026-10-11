#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores a flag at +0x1C. On the first call (byte at +0 still zero), copies two
// option bytes into +0x15/+0x16, dispatches virtual slot 22 on the object at +4
// with (self, a2), and latches whether that dispatch returned -1 into the byte at +0.
// Returns the stored flag (or the dispatch outcome on the first call).
static int __cdecl SetFlagAndDispatchOnce(void* self, int a2, int a3, char a4, char a5)
{
    int result = a3;
    CSF_AT(int, self, 0x1C) = a3;

    if (CSF_AT(uint8_t, self, 0) == 0)
    {
        CSF_AT(char, self, 0x15) = a4;
        void* target = CSF_AT(void*, self, 0x04);
        CSF_AT(char, self, 0x16) = a5;

        // __thiscall: ecx = target, first stack arg = self, second = a2.
        int dispatchResult = ((int(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, target, 0)[0x58 / 4])(target, nullptr, self, a2);

        result = (dispatchResult == -1) ? 1 : 0;
        CSF_AT(uint8_t, self, 0) = (uint8_t)result;
    }
    return result;
}

HOOK(0x407450, SetFlagAndDispatchOnce, "auto");
