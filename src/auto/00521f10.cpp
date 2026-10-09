#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// Starts the active sound instance held by the global sound manager, if one exists.
// Returns the instance handle (0 if none), or the result of the instance's start call.
static int __cdecl SoundMgr_StartActiveInstanceIfReady()
{
    int managerHandle = GetGlobalValue_70DDE0();
    void* manager = reinterpret_cast<void*>(managerHandle);

    // Slot 28 (+0x70): obtain the active sound instance from the manager.
    int activeInstance = reinterpret_cast<int(__fastcall*)(void*, void*)>(
        CSF_AT(void**, manager, 0)[28])(manager, nullptr);

    int result = activeInstance;
    if (result)
    {
        void* instance = reinterpret_cast<void*>(activeInstance);

        // Slot 15 (+0x3C): query whether the instance is ready, argument 1.
        result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, instance, 0)[15])(instance, nullptr, 1);
        if (result)
        {
            // Slot 6 (+0x18): start the instance, argument 1.
            return reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, instance, 0)[6])(instance, nullptr, 1);
        }
    }
    return result;
}

HOOK(0x521F10, SoundMgr_StartActiveInstanceIfReady, "auto");
