#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_FUNC(uint8_t*, __cdecl, SceneLoader_CreateLoadTimer, 0x0069CDC0, ());

CSF_GLOBAL(uint32_t, g_NetProfileLoadTimer, 0x008B065C);
CSF_GLOBAL(uint32_t, g_NetProfileStateBlock, 0x008B0590);

// Creates the scene load timer, stores its handle, and zeroes the 0xCC-byte
// net profile state block that follows the stored data.
static int NetProfile_ResetLoadState()
{
    g_NetProfileLoadTimer = reinterpret_cast<uintptr_t>(SceneLoader_CreateLoadTimer());
    std::memset(&g_NetProfileStateBlock, 0, 0xCC);
    return 0;
}

HOOK(0x00519120, NetProfile_ResetLoadState, "auto");
