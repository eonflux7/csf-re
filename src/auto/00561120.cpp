#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

CSF_FUNC(bool, __cdecl, MenuMpSessions_GetSessionNetworkFlag, 0x00749730, (uint8_t* a1));
CSF_FUNC(bool, __cdecl, MenuMpSessions_GetSessionPrimaryFlag, 0x00749750, (int a1));
CSF_FUNC(int, __cdecl, MenuMpSessions_GetSessionCategory, 0x00749770, (int a1));
CSF_FUNC(bool, __cdecl, MenuMpSessions_GetSessionSecondaryFlag, 0x007497E0, (int a1));
CSF_FUNC(bool, __cdecl, MenuMpSessions_IsSessionAvailable, 0x00749800, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static bool __fastcall MenuMpSessions_MatchesSessionFilters(
    void* self, void* /*edx*/, int sessionIndex)
{
    const int session = sessionIndex + 3;
    auto* sessionArgument =
        reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(session));

    bool result;
    if (g_MpNetworkType == 2)
        result = MenuMpSessions_GetSessionNetworkFlag(sessionArgument);
    else
        result = !MenuMpSessions_GetSessionNetworkFlag(sessionArgument);

    if (!result)
        return result;

    switch (CSF_AT(int, self, 0x18))
    {
    case 0:
    case 1:
    case 3:
    {
        const int category = MenuMpSessions_GetSessionCategory(session);
        result = category == CSF_AT(int, self, 0x18);
        break;
    }
    case 5:
        result = true;
        break;
    default:
        break;
    }

    if (!result)
        return result;

    switch (CSF_AT(int, self, 0x1C))
    {
    case 0:
        result = true;
        break;
    case 1:
        result = MenuMpSessions_GetSessionPrimaryFlag(session);
        break;
    case 2:
        result = !MenuMpSessions_GetSessionPrimaryFlag(session);
        break;
    default:
        break;
    }

    if (!result)
        return result;

    switch (CSF_AT(int, self, 0x1C0))
    {
    case 0:
        break;
    case 1:
        result = MenuMpSessions_GetSessionSecondaryFlag(session);
        break;
    case 2:
        result = !MenuMpSessions_GetSessionSecondaryFlag(session);
        break;
    default:
        break;
    }

    if (!result)
        return result;

    const int globalValue = GetGlobalValue_8BC1E0();
    if (CSF_AT(uint8_t, reinterpret_cast<void*>(globalValue), 0x1D6C))
        return true;

    result = MenuMpSessions_IsSessionAvailable(session);
    if (result)
        return true;

    return result;
}

HOOK(0x00561120, MenuMpSessions_MatchesSessionFilters, "auto");
