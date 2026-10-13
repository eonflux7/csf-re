#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(int, g_globalValue_8B0560, 0x008B0560);

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_00830438_ConstructDevConsole, 0x004FB240, (C_00830438* self, void*));
CSF_FUNC(void, __fastcall, C_0085ec18_InitializeConsoleResources, 0x004FA6C0, (C_0085ec18* self, void*));

static char __cdecl DevConsole_EnsureInitialized()
{
    if (!g_globalValue_8B0560)
    {
        int rawStorage = CoreMemory_AllocUntagged(460);
        if (rawStorage)
        {
            C_00830438* consoleObject = reinterpret_cast<C_00830438*>(
                static_cast<std::uintptr_t>(static_cast<std::uint32_t>(rawStorage)));
            // ecx for the next call is the constructor's return value (mov ecx, eax after the call).
            int constructed = C_00830438_ConstructDevConsole(consoleObject, nullptr);
            g_globalValue_8B0560 = constructed;
            // Still ecx = constructed here: pass that same value as `this`.
            C_0085ec18* resources = reinterpret_cast<C_0085ec18*>(
                static_cast<std::uintptr_t>(static_cast<std::uint32_t>(constructed)));
            C_0085ec18_InitializeConsoleResources(resources, nullptr);
            return 1;
        }
        // Failure path: xor eax,eax / mov ecx,eax -> ecx is 0 when the original calls 0x4FA6C0.
        std::uint32_t ecxValue = 0;
        g_globalValue_8B0560 = 0;
        C_0085ec18_InitializeConsoleResources(
            reinterpret_cast<C_0085ec18*>(static_cast<std::uintptr_t>(ecxValue)), nullptr);
    }
    return 1;
}

HOOK(0x004FB690, DevConsole_EnsureInitialized, "auto");
