#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_65E430, 0x0065E430, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, sub_64C610, 0x0064C610, (int a1));
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));

CSF_GLOBAL(uint32_t, g_NetGameClassId, 0x008BC178);

static int __cdecl NetGameDDL_DispatchOrRaiseError(int a1)
{
    if ((uint8_t)sub_65E430(reinterpret_cast<void*>(a1), nullptr, static_cast<int>(g_NetGameClassId)))
        return sub_64C610(a1);

    Game_RaiseStatusNotice(0, 0, 0xE0030007u, 0);
    return 0;
}

HOOK(0x0074C520, NetGameDDL_DispatchOrRaiseError, "auto");
