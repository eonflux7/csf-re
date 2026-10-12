#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __cdecl RecordFlagAndDispatchOnce(void* self, int argument, int flag,
                                            char optionA, char optionB)
{
    int result = flag;
    CSF_AT(int, self, 0x1C) = flag;

    if (CSF_AT(uint8_t, self, 0) == 0)
    {
        CSF_AT(char, self, 0x15) = optionA;
        void* target = CSF_AT(void*, self, 0x04);
        CSF_AT(char, self, 0x16) = optionB;

        int dispatchResult = ((int(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, target, 0)[0x4C / 4])(target, nullptr, argument, self);

        result = dispatchResult == -1 ? 1 : 0;
        CSF_AT(uint8_t, self, 0) = static_cast<uint8_t>(result);
    }

    return result;
}

HOOK(0x4722C0, RecordFlagAndDispatchOnce, "auto");
