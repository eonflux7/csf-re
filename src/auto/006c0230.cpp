#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

using GfxFrames_CallbackFn = void(__cdecl*)(int);

// Calls the callback stored at object+0x48 with the object as its only argument,
// preserving bit 2 (0x04) of the flags byte at object+0x02 from before the call.
static char __cdecl GfxFrames_InvokeCallbackPreservingFlag4(int a1) {
    uint8_t* obj = reinterpret_cast<uint8_t*>(a1);

    const bool flagWasSet = (CSF_AT(uint8_t, obj, 0x02) & 0x04) != 0;
    CSF_AT(uint8_t, obj, 0x02) = static_cast<uint8_t>(CSF_AT(uint8_t, obj, 0x02) | 0x04);

    GfxFrames_CallbackFn callback = CSF_AT(GfxFrames_CallbackFn, obj, 0x48);
    callback(a1);

    uint8_t result = CSF_AT(uint8_t, obj, 0x02);
    if (flagWasSet) {
        result = static_cast<uint8_t>(result | 0x04);
    } else {
        result = static_cast<uint8_t>(result & 0xFB);
    }
    CSF_AT(uint8_t, obj, 0x02) = result;
    return static_cast<char>(result);
}

HOOK(0x006C0230, GfxFrames_InvokeCallbackPreservingFlag4, "auto");
