#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(std::uint8_t, g_OnlineMatchFlag, 0x008B068C);

CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __cdecl, ReturnZero, 0x004B2A30, ());

// Reads the online/offline state value (via a virtual getter in online mode,
// or a plain global/zero in offline mode) and caches it in the owner's
// field at +0x674 when it changes.
static int __stdcall HudAlertIcons_RefreshIconState(int a1)
{
    int result;

    if (g_OnlineMatchFlag != 0) {
        int globalState = GetGlobalState_6A6D70();
        void* stateObj = reinterpret_cast<void*>(static_cast<std::uintptr_t>(globalState));
        // vtable slot at byte offset 0xF0 -> index 0xF0 / 4 = 60
        result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, stateObj, 0)[60])(stateObj, nullptr);
    } else {
        int globalDword = GetGlobalDword_73C020();
        result = ReturnZero();
    }

    void* self = reinterpret_cast<void*>(static_cast<std::uintptr_t>(a1));
    if (result != CSF_AT(int, self, 0x674)) {
        CSF_AT(int, self, 0x674) = result;
    }
    return result;
}

HOOK(0x004A1C80, HudAlertIcons_RefreshIconState, "auto");
