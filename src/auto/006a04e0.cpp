#include <cstdint>
#include "globals.h"
#include "hook.h"

// Win32 GetFileAttributesA, reached through its import-address-table slot.
using GetFileAttributesAFn = uint32_t(__stdcall*)(const char* lpFileName);
CSF_GLOBAL(GetFileAttributesAFn, g_GetFileAttributesAImport, 0x82D22C);

static constexpr uint32_t kInvalidFileAttributes = 0xFFFFFFFFu;

// Returns true when the path is non-null, non-empty and the OS reports
// attributes for it (the file or directory exists).
static bool __cdecl CoreLog_PathExists(const char* lpFileName)
{
    bool result = false;
    if (lpFileName)
    {
        if (*lpFileName)
            return g_GetFileAttributesAImport(lpFileName) != kInvalidFileAttributes;
    }
    return result;
}

HOOK(0x6A04E0, CoreLog_PathExists, "auto");
