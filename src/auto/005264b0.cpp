#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// Gets the global audio manager, asks it for its active subsystem (vtable slot 0x70),
// and if one exists forwards the sound request to it (vtable slot 0x0C) with flag 1.
static int __stdcall AudioSoundManager_ForwardToActiveSubsystem(int a1) {
    void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());

    using GetSubsystemFn = void* (__fastcall*)(void* self, void* edx);
    void* subsystem = ((GetSubsystemFn)CSF_AT(void**, manager, 0)[0x70 / 4])(manager, nullptr);
    if (subsystem == nullptr) {
        return 0;
    }

    using ForwardSoundFn = int (__fastcall*)(void* self, void* edx, int sound, int flag);
    return ((ForwardSoundFn)CSF_AT(void**, subsystem, 0)[0x0C / 4])(subsystem, nullptr, a1, 1);
}

HOOK(0x5264B0, AudioSoundManager_ForwardToActiveSubsystem, "auto");
