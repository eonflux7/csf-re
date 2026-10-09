#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

// 12-byte stack block the original fills before the final virtual call:
// [0] float weight = 1.0f, [4] int zero, [8] flagA = 1, [9] flagB = 0.
struct SharedTextParams {
    float weight;
    int32_t zero;
    uint8_t flagA;
    uint8_t flagB;
};

static int __stdcall OnlineMenu_SetSharedText(int a1)
{
    int globalValue = GetGlobalValue_70DDE0();
    void* globalObj = reinterpret_cast<void*>(static_cast<uintptr_t>(globalValue));

    // globalObj->vtbl[0x70 / 4]() : obtain the shared sound-manager object
    int soundHandle = ((int(__fastcall*)(void*, void*))CSF_AT(void**, globalObj, 0)[0x70 / 4])(globalObj, nullptr);
    int result = soundHandle;

    if (soundHandle != 0) {
        void* sound = reinterpret_cast<void*>(static_cast<uintptr_t>(soundHandle));

        // sound->vtbl[0x3C / 4](0) : query whether the sound is busy
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, sound, 0)[0x3C / 4])(sound, nullptr, 0);

        if (result == 0) {
            SharedTextParams params{};
            params.weight = 1.0f;
            params.zero = 0;
            params.flagA = 1;
            params.flagB = 0;

            // sound->vtbl[4 / 4](a1, &params)
            result = ((int(__fastcall*)(void*, void*, int, SharedTextParams*))CSF_AT(void**, sound, 0)[4 / 4])(sound, nullptr, a1, &params);
        }
    }

    return result;
}

HOOK(0x521E30, OnlineMenu_SetSharedText, "auto");
