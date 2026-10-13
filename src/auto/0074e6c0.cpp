#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_65E430, 0x0065E430, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, NetDdl_BuildPlayerEvent, 0x0064C610, (int a1));
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));

CSF_GLOBAL(uint32_t, g_NetControlClassId, 0x008BC17C);

static int __cdecl NetDdl_CreateSelectionEvent(int a1)
{
    if ((uint8_t)sub_65E430(reinterpret_cast<void*>(a1), nullptr, static_cast<int>(g_NetControlClassId)))
        return NetDdl_BuildPlayerEvent(a1);

    Game_RaiseStatusNotice(0, 0, 0xE0030007u, 0);
    return 0;
}

HOOK(0x0074E6C0, NetDdl_CreateSelectionEvent, "auto");
