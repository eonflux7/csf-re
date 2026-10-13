#include <cstddef>

#include "globals.h"
#include "hook.h"

// Engine bindings (addresses from the original binary).
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, RwImageRegisterImageFormat, 0x00625680, (int a1, int a2, int a3));

// Global holding the resource subsystem block allocated on first init.
CSF_GLOBAL(void*, g_ResourceSubsystemBlock, 0x008BB894);

// Returns true only when both image formats were registered.
// Already-initialized path returns false without touching anything.
static bool Im3d_InitializeResourceSubsystem() {
    bool result = false;
    if (!g_ResourceSubsystemBlock) {
        g_ResourceSubsystemBlock = Mem_Alloc(1);

        int registerStatus = RwImageRegisterImageFormat(
            static_cast<int>(CSF_ADDR_DATA(0x008631BC)),
            static_cast<int>(CSF_ADDR_FN(0x007C9050)),
            static_cast<int>(CSF_ADDR_FN(0x007C8D30)));
        if (registerStatus) {
            registerStatus = RwImageRegisterImageFormat(
                static_cast<int>(CSF_ADDR_DATA(0x008631B8)),
                static_cast<int>(CSF_ADDR_FN(0x007C5680)),
                static_cast<int>(CSF_ADDR_FN(0x007C5CA0)));
        }
        return registerStatus != 0;
    }
    return result;
}

HOOK(0x006E2E60, Im3d_InitializeResourceSubsystem, "auto");
