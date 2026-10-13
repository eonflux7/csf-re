#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callee: returns nonzero when file info was filled in.
CSF_FUNC(char, __cdecl, CErrorLog_TryFillFileInfo, 0x0069D3D0, (int a1, char* a2));

// Inline "No File Info" literal (13 bytes incl. NUL), stored as dwords/byte in .data.
CSF_GLOBAL(uint32_t, g_NoFileInfoWord0, 0x0085E9CC);
CSF_GLOBAL(uint32_t, g_NoFileInfoWord1, 0x0085E9D0);
CSF_GLOBAL(uint32_t, g_NoFileInfoWord2, 0x0085E9D4);
CSF_GLOBAL(uint8_t,  g_NoFileInfoTerminator, 0x0085E9D8);

static char* __cdecl CErrorLog_GetFileInfoOrDefault(int a1, char* a2)
{
    if (!CErrorLog_TryFillFileInfo(a1, a2)) {
        CSF_AT(uint32_t, a2, 0x0) = g_NoFileInfoWord0;
        CSF_AT(uint32_t, a2, 0x4) = g_NoFileInfoWord1;
        CSF_AT(uint32_t, a2, 0x8) = g_NoFileInfoWord2;
        CSF_AT(uint8_t,  a2, 0xC) = g_NoFileInfoTerminator;
    }
    return a2;
}

HOOK(0x0069D4B0, CErrorLog_GetFileInfoOrDefault, "auto");
