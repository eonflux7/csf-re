#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callback invoked by SOUNDMAX with a 5-dword effect record.
using AudioEffectCallback = void(__cdecl*)(uint32_t* effectRecord);

// Reads the effect block at self+0x50, builds a 5-dword record from it and the
// callback slot's counter, calls the callback, then increments the slot's counter.
static int __cdecl Audio_InvokeEffectCallback(int a1, int a2) {
    uint8_t* self = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a1));

    uint32_t effectBlockValue = CSF_AT(uint32_t, self, 0x50);
    if (effectBlockValue == 0)
        return a1;
    if (a2 == 0)
        return a1;

    uint8_t* callbackSlot = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
    AudioEffectCallback callback = CSF_AT(AudioEffectCallback, callbackSlot, 0);
    if (callback == nullptr)
        return a1;

    uint8_t* effectBlock = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(effectBlockValue));
    uint32_t params[7];
    std::memcpy(params, effectBlock + 0x2C, sizeof(params));

    uint32_t effectRecord[5];
    effectRecord[0] = CSF_AT(uint32_t, effectBlock, 4);
    effectRecord[1] = params[2];
    effectRecord[2] = params[0];
    effectRecord[3] = (params[3] >> 8) & 0xFF;
    effectRecord[4] = CSF_AT(uint32_t, callbackSlot, 4);

    callback(effectRecord);

    ++CSF_AT(uint32_t, callbackSlot, 4);
    return a1;
}

HOOK(0x70FBB0, Audio_InvokeEffectCallback, "auto");
