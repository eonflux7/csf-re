#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(char*, __cdecl, GetSelectedCharacterSpawnName, 0x00551310, ());
CSF_FUNC(int, __cdecl, GetMpGameModeIndex, 0x005515F0, ());

static int __fastcall CMenuMpLoadout_RefreshSpawnOptions(
    CMenuMpLoadout* self, void* /*edx*/, int team)
{
    void* list = reinterpret_cast<void*>(self->field_0028);
    if (!list)
        return 0;

    const int previousSelection =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, list, 0)[0x2AC / 4])(list, nullptr);

    list = reinterpret_cast<void*>(self->field_0028);
    reinterpret_cast<void (__fastcall*)(void*, void*)>(
        CSF_AT(void**, list, 0)[0x2B8 / 4])(list, nullptr);

    GetGlobal8B0688();
    const char firstEnabled = GetSelectedCharacterSpawnName()[0];
    GetGlobal8B0688();
    const char secondEnabled = GetSelectedCharacterSpawnName()[1];
    GetGlobal8B0688();
    const char thirdEnabled = GetSelectedCharacterSpawnName()[2];

    const bool alternateTeam = team == 2;
    GetGlobal8B0688();
    switch (GetMpGameModeIndex())
    {
    case 0:
    case 1:
    case 3:
        if (firstEnabled)
        {
            list = reinterpret_cast<void*>(self->field_0028);
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, list, 0)[0x29C / 4])(
                    list, nullptr, alternateTeam ? 1229861177 : 1229860918);
        }
        if (secondEnabled)
        {
            list = reinterpret_cast<void*>(self->field_0028);
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, list, 0)[0x29C / 4])(
                    list, nullptr, alternateTeam ? 1229861424 : 1229860919);
        }
        if (!thirdEnabled)
            break;
        [[fallthrough]];

    case 2:
        list = reinterpret_cast<void*>(self->field_0028);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, list, 0)[0x29C / 4])(
                list, nullptr, alternateTeam ? 1229861425 : 1229860920);
        break;

    default:
        break;
    }

    list = reinterpret_cast<void*>(self->field_0028);
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, list, 0)[0x2A8 / 4])(
            list, nullptr, previousSelection);
}

HOOK(0x0052EDE0, CMenuMpLoadout_RefreshSpawnOptions, "auto");
