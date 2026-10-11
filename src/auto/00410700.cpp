#include <cstdint>
#include "globals.h"
#include "hook.h"

// Stores a flag at +0x1C, and on the first call latches the outcome of a
// virtual dispatch (slot 18 on the object at +4). Returns the stored flag.
static int __cdecl RecordStateAndDispatchOnce(void* self, int a2, int a3, char a4, char a5)
{
    int result = a3;
    CSF_AT(int, self, 0x1C) = a3;

    if (CSF_AT(uint8_t, self, 0) == 0)
    {
        CSF_AT(char, self, 0x15) = a4;
        void* target = CSF_AT(void*, self, 0x04);
        CSF_AT(char, self, 0x16) = a5;

        int dispatchResult = ((int(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, target, 0)[0x48 / 4])(target, nullptr, a2, self);

        result = (dispatchResult == -1) ? 1 : 0;
        CSF_AT(uint8_t, self, 0) = (uint8_t)result;
    }
    return result;
}

HOOK(0x410700, RecordStateAndDispatchOnce, "auto");
