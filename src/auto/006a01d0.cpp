#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RtFs_SetDefaultFileSystem, 0x007C6E90, (int a1, int a2, int a3));
CSF_FUNC(int, __cdecl, RtFs_ValidateFileSystemHandle, 0x007C6350, (uint32_t* a1));

CSF_GLOBAL(const char*, g_psz_win, 0x008A6BA4);
CSF_GLOBAL(void*, g_pfnGetCurrentDirectoryA, 0x0082D194);

static bool __fastcall C_0085ec34_SetDefaultFileSystemFromCwd(
    C_0085ec34* self, void* /*edx*/, int fileSystemId, int /*unused1*/, int /*unused2*/)
{
    char cwdBuffer[0xC8];

    // Keep ECX pointing to self at the import call, while passing both
    // GetCurrentDirectoryA arguments on the stack (callee pops eight bytes).
    using GetCurrentDirectoryCall =
        uint32_t(__fastcall*)(C_0085ec34*, void*, uint32_t, char*);
    reinterpret_cast<GetCurrentDirectoryCall>(g_pfnGetCurrentDirectoryA)(
        self, nullptr, 0xC8u, cwdBuffer);

    cwdBuffer[2] = 0;
    const uint32_t fileSystemHandle = static_cast<uint32_t>(
        RtFs_SetDefaultFileSystem(
            fileSystemId,
            static_cast<int>(reinterpret_cast<uintptr_t>(cwdBuffer)),
            static_cast<int>(reinterpret_cast<uintptr_t>(g_psz_win))));

    self->field_0004 = fileSystemHandle;
    return fileSystemHandle != 0 &&
           RtFs_ValidateFileSystemHandle(
               reinterpret_cast<uint32_t*>(fileSystemHandle)) != 0;
}

HOOK(0x006A01D0, C_0085ec34_SetDefaultFileSystemFromCwd, "auto");
