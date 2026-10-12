#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, DevConsole_CleanupTemporaryMenuResource, 0x004FB970, (void* self, void* edx));

static int __fastcall DevConsole_DestroyTemporaryMenuResource(void* self, void* /*edx*/)
{
    int result = 0;
    if (CSF_AT(std::uint8_t, self, 0x00))
    {
        result = DevConsole_CleanupTemporaryMenuResource(self, nullptr);
        CSF_AT(std::uint8_t, self, 0x00) = 0;
    }
    return result;
}

HOOK(0x004FBA10, DevConsole_DestroyTemporaryMenuResource, "auto");
