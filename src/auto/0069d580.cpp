#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Import-address-table slot for shell32!SHGetFolderPathW (called indirectly at 0x69D599).
using ShGetFolderPathWFn = int32_t(__stdcall*)(void* hwnd, int csidl, void* hToken, uint32_t flags, wchar_t* pszPath);
CSF_GLOBAL(ShGetFolderPathWFn, g_ShGetFolderPathW, 0x82D268);

// Returns pszPath on success (HRESULT >= 0), 0 on failure or when pszPath is null.
// CSIDL 5 = CSIDL_PERSONAL (My Documents); the caller receives the buffer pointer back.
static unsigned int __cdecl ProfilesSavegames_GetBaseDirectory(wchar_t* pszPath)
{
    if (!pszPath)
        return 0;

    const int32_t hresult = g_ShGetFolderPathW(nullptr, 5, nullptr, 0, pszPath);
    if (hresult < 0)
        return 0;

    return static_cast<unsigned int>(reinterpret_cast<uintptr_t>(pszPath));
}

HOOK(0x0069D580, ProfilesSavegames_GetBaseDirectory, "auto");
