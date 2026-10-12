#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));

static int __fastcall MpSpawnPoints_GetInteractionMode(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x2C);
    if ((result & 0x3FFFFF) == 0)
    {
        Game_RaiseStatusNotice(0, 0, 0xE000000E, 0);
        return 0;
    }
    return result;
}

HOOK(0x00550CA0, MpSpawnPoints_GetInteractionMode, "auto");
