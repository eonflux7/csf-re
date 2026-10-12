#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (std::uint16_t* source, char* destination, unsigned int length));
CSF_FUNC(bool, __cdecl, MenuMpSessions_HasNetworkTypeTwo, 0x00749730, (uint8_t* a1));
CSF_FUNC(bool, __cdecl, MenuMpSessions_HasPrimaryFilterFlag, 0x00749750, (int a1));
CSF_FUNC(int, __cdecl, MenuMpSessions_GetSessionMode, 0x00749770, (int a1));
CSF_FUNC(bool, __cdecl, MenuMpSessions_HasSecondaryFilterFlag, 0x007497E0, (int a1));
CSF_FUNC(bool, __cdecl, MenuMpSessions_PassesFinalSessionCheck, 0x00749800, (int a1));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

static int __fastcall MenuMpSessions_MatchesSessionFilters(
    void* self, void* /*edx*/, void* session)
{
    alignas(8) uint8_t text[0x80] = {};
    ConvertWideTextToNarrow(
        reinterpret_cast<std::uint16_t*>(
            reinterpret_cast<uint8_t*>(session) + 4),
        reinterpret_cast<char*>(text), 127);

    bool matches;
    if (g_MpNetworkType == 2)
        matches = MenuMpSessions_HasNetworkTypeTwo(text);
    else
        matches = !MenuMpSessions_HasNetworkTypeTwo(text);

    if (!matches)
        return matches;

    const int textAddress = static_cast<int>(
        reinterpret_cast<std::intptr_t>(text));

    switch (CSF_AT(int, self, 0x18))
    {
    case 0:
    case 1:
    case 3:
    {
        const int sessionMode = MenuMpSessions_GetSessionMode(textAddress);
        matches = sessionMode == CSF_AT(int, self, 0x18);
        break;
    }
    case 5:
        matches = true;
        break;
    default:
        break;
    }

    if (!matches)
        return matches;

    switch (CSF_AT(int, self, 0x1C))
    {
    case 0:
        matches = true;
        break;
    case 1:
        matches = MenuMpSessions_HasPrimaryFilterFlag(textAddress);
        break;
    case 2:
        matches = !MenuMpSessions_HasPrimaryFilterFlag(textAddress);
        break;
    default:
        break;
    }

    if (!matches)
        return matches;

    switch (CSF_AT(int, self, 0x1C0))
    {
    case 0:
        break;
    case 1:
        matches = MenuMpSessions_HasSecondaryFilterFlag(textAddress);
        break;
    case 2:
        matches = !MenuMpSessions_HasSecondaryFilterFlag(textAddress);
        break;
    default:
        break;
    }

    if (!matches)
        return matches;

    void* globalState = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(GetGlobalValue_8BC1E0()));
    if (CSF_AT(uint8_t, globalState, 0x1D6C))
        return 1;

    matches = MenuMpSessions_PassesFinalSessionCheck(textAddress);
    if (matches)
        return 1;
    return matches;
}

HOOK(0x00560FD0, MenuMpSessions_MatchesSessionFilters, "auto");
