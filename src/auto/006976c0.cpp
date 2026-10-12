#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __cdecl, PrefixBaseDirectoryIfRelative, 0x006975F0, (char* path));

CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

using SetFileAttributesProc = int (__stdcall*)(const char*, uint32_t);
using DeleteFileProc = int (__stdcall*)(const char*);
CSF_GLOBAL(SetFileAttributesProc, g_SetFileAttributesA, 0x0082D118);
CSF_GLOBAL(DeleteFileProc, g_DeleteFileA, 0x0082D210);

static bool __cdecl CoreFiles_DeleteFile(const char* path)
{
    char fileName[0x100];
    char* destination = fileName;
    do
    {
        *destination++ = *path;
    } while (*path++ != '\0');

    if (!CSF_AT(uint8_t, g_pOptions, 0x68))
        PrefixBaseDirectoryIfRelative(fileName);

    g_SetFileAttributesA(fileName, 0x20u);
    return g_DeleteFileA(fileName) != 0;
}

HOOK(0x006976C0, CoreFiles_DeleteFile, "auto");
