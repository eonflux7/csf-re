#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_SetFilePointerImport, 0x82D230);

static int __cdecl Audio_GetFileCurrentPosition(void* hFile)
{
    return ((int(__stdcall*)(void*, uint32_t, uint32_t, uint32_t))g_SetFilePointerImport)(hFile, 0, 0, 1);
}

HOOK(0x70E020, Audio_GetFileCurrentPosition, "auto");
