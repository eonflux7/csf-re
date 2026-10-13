#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"

// Stored default file-system handle (set elsewhere in the game).
CSF_GLOBAL(int, g_008bb400, 0x008BB400);

// RtFS manager: installs a file system as the default one.
CSF_FUNC(int, __cdecl, RtFSManagerSetDefaultFileSystem, 0x007C61A0, (int fileSystem));

// Installs the configured default file system if one has been set.
// Returns the result of RtFSManagerSetDefaultFileSystem, or the (zero) stored
// handle when nothing is configured.
static int RtFS_ApplyConfiguredDefaultFileSystem()
{
    int configuredFileSystem = g_008bb400;
    if (configuredFileSystem)
        return RtFSManagerSetDefaultFileSystem(configuredFileSystem);
    return configuredFileSystem;
}

HOOK(0x0069F9D0, RtFS_ApplyConfiguredDefaultFileSystem, "auto");
