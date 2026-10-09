#include "globals.h"
#include "hook.h"

CSF_GLOBAL(const char*, g_psz_SOLDADO, 0x008995DC);
CSF_GLOBAL(const char*, g_psz_SUBOFICIAL, 0x008995E0);
CSF_GLOBAL(const char*, g_psz_OFICIAL, 0x008995E4);
CSF_GLOBAL(const char*, g_psz_GENERAL, 0x008995E8);
CSF_GLOBAL(const char*, g_psz_GESTAPO, 0x008995EC);

CSF_FUNC(int, __cdecl, __stricmp, 0x007B0C41, (const char* left, const char* right));
CSF_FUNC(bool, __cdecl, RankNames_MatchesGestapoName, 0x0042D5F0, (char* String1, char* String2));

// Maps a rank name to its index: 0 SOLDADO, 1 SUBOFICIAL, 2 OFICIAL, 3 GENERAL,
// 4 when the GESTAPO comparison succeeds, -1 otherwise.
static int __stdcall Hud_GetRankIndexByName(char* String1)
{
    if (String1 != nullptr)
    {
        if (g_psz_SOLDADO != nullptr && __stricmp(String1, g_psz_SOLDADO) == 0)
            return 0;
        if (g_psz_SUBOFICIAL != nullptr && __stricmp(String1, g_psz_SUBOFICIAL) == 0)
            return 1;
        if (g_psz_OFICIAL != nullptr && __stricmp(String1, g_psz_OFICIAL) == 0)
            return 2;
        if (g_psz_GENERAL != nullptr && __stricmp(String1, g_psz_GENERAL) == 0)
            return 3;
    }

    // Reached with String1 possibly null; the original still makes this call.
    bool matchesGestapo = RankNames_MatchesGestapoName(String1, const_cast<char*>(g_psz_GESTAPO));
    if (!matchesGestapo)
        return -1;
    return 4;
}

HOOK(0x0042D8B0, Hud_GetRankIndexByName, "auto");
