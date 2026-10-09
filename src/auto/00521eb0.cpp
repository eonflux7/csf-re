#include <cstdint>
#include <cstddef>

#include "hook.h"
#include "globals.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// Parameter block passed to the sound manager's play slot.
// Layout matches the original stack frame: float at -0xC, int at -0x8, bytes at -0x4 and -0x3.
struct SoundPlayParams {
    float volume;        // [ebp-0xC] = 1.0f
    int32_t loopCount;   // [ebp-0x8] = 1
    uint8_t flagA;       // [ebp-0x4] = al (0 on this path)
    uint8_t flagB;       // [ebp-0x3] = 1
    uint8_t pad[2];
};

// Plays a sound through the global sound manager when it is available and not blocked.
// Returns 0 if the manager is missing or blocked; otherwise the result of the play call.
static int __stdcall TryPlaySoundInManager(int soundId) {
    void* soundManager = reinterpret_cast<void*>(
        static_cast<intptr_t>(GetGlobalValue_70DDE0()));

    // Slot 28 (+0x70) on the global value object: fetch the active sound manager.
    void* activeManager = nullptr;
    activeManager = reinterpret_cast<void*>(static_cast<intptr_t>(
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, soundManager, 0)[28 ])(soundManager, nullptr)));
    if (!activeManager) {
        return 0;
    }

    // Slot 15 (+0x3C) on the manager with argument 1: query whether playback is blocked.
    int blockedResult = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, activeManager, 0)[15])(
        activeManager, nullptr, 1);
    if (blockedResult != 0) {
        return blockedResult;
    }

    SoundPlayParams params{};
    params.volume = 1.0f;
    params.loopCount = 1;
    params.flagA = 0;
    params.flagB = 1;

    // Slot 1 (+0x4) on the manager: play sound with the parameter block.
    return ((int(__fastcall*)(void*, void*, int, SoundPlayParams*))CSF_AT(void**, activeManager, 0)[1])(
        activeManager, nullptr, soundId, &params);
}

HOOK(0x521EB0, TryPlaySoundInManager, "auto");
