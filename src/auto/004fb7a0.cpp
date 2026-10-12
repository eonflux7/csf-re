#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, DevConsole_GetToken, 0x004FB710,
         (void* self, void* edx, unsigned int a2, uint8_t* a3));
CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41,
         (const char* left, const char* right));

static char __fastcall S_004fb710_GetValueAfterToken(
    S_004fb710* self, void* /*edx*/, char* token, uint8_t* output)
{
    alignas(8) uint8_t tokenBuffer[0x100] = {};
    unsigned int index = 0;
    *output = 0;

    while (DevConsole_GetToken(self, nullptr, index++, tokenBuffer))
    {
        if (__stricmp(reinterpret_cast<const char*>(tokenBuffer), token) == 0)
        {
            DevConsole_GetToken(self, nullptr, index, output);
            return 1;
        }
    }
    return 0;
}

HOOK(0x004FB7A0, S_004fb710_GetValueAfterToken, "auto");
