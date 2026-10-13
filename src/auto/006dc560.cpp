#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Brightness/gamma ramp: builds three 256-entry WORD channels (R, G, B) that ramp
// from 0 up to 0xFFFF around the level (brightness - 100), then calls the device's
// slot 0x54 (stdcall, 4 args) with the device's vtable pointer left in ecx, as the
// original does. Returns 0 if the device pointer is null, otherwise the call's result.
static int __fastcall C_00861460_ApplyGammaRamp(C_00861460* self, void* /*edx*/, int brightness)
{
    if (!self->field_005c)
        return 0;

    // 0x604-byte scratch buffer: three 0x200-byte channel rows at offsets 0x000, 0x200, 0x400.
    alignas(8) uint8_t rampBytes[0x604];
    uint16_t* ramp = reinterpret_cast<uint16_t*>(rampBytes);

    int level = brightness - 100;
    for (int i = 0; i < 256; ++i, ++level)
    {
        int clamped = level;
        if (clamped < 0)
            clamped = 0;
        else if (clamped > 255)
            clamped = 255;

        int channelValue = (0xFFFF * clamped) / 255;

        ramp[i]       = static_cast<uint16_t>(channelValue); // row 0
        ramp[256 + i] = static_cast<uint16_t>(channelValue); // row 1
        ramp[512 + i] = static_cast<uint16_t>(channelValue); // row 2
    }

    // The original reloads field_005c after the loop.
    void* device = reinterpret_cast<void*>(self->field_005c);

    // Stdcall target with 4 stack args (device, 0, 1, buffer). Modelled as a fastcall
    // pointer so that ecx = device vtable pointer and edx = buffer, matching the original's
    // register state at the indirect call. The callee pops its own 16 bytes.
    using DeviceRampFn = int(__fastcall*)(void* ecxVtable, void* edxBuffer,
                                          void* deviceArg, int zero, int one, void* bufferArg);
    DeviceRampFn deviceRamp = reinterpret_cast<DeviceRampFn>(CSF_AT(void**, device, 0)[0x54 / 4]);
    void* deviceVtable = CSF_AT(void*, device, 0);
    return deviceRamp(deviceVtable, rampBytes, device, 0, 1, rampBytes);
}

HOOK(0x006DC560, C_00861460_ApplyGammaRamp, "auto");
