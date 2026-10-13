#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Calls the helper at 0x712F60 with the device pointer in esi (no stack args).
static char CallAcquireHelper712F60(uint32_t device) {
    hook::Regs r{};
    r.esi = device;
    uint32_t eax = hook::call_regs(CSF_ADDR_FN(0x00712F60), &r, nullptr, 0);
    return static_cast<char>(eax);
}

// Virtual slot +0x20 (slot 8) on the device object, stdcall, one stack argument (the device itself).
static char CallDeviceSlot8(uint32_t deviceAddr) {
    void* device = reinterpret_cast<void*>(deviceAddr);
    using SlotEightFn = char(__stdcall*)(void*);
    SlotEightFn slotFn = reinterpret_cast<SlotEightFn>(CSF_AT(void**, device, 0)[8]);
    return slotFn(device);
}

static char __fastcall CInputDirectInput_SetAcquireEnabled(CInputDirectInput* self, void* /*edx*/, char a2) {
    char result = a2;

    if (self->field_0c90 != static_cast<uint8_t>(a2)) {
        self->field_0c90 = static_cast<uint8_t>(a2);

        if (a2) {
            uint32_t firstDevice = self->field_0020;
            if (firstDevice) {
                self->field_0028 = 0;
                result = CallAcquireHelper712F60(firstDevice);
            }

            uint32_t secondDevice = self->field_0024;
            if (secondDevice) {
                result = static_cast<char>(self->field_0c91);
                if (result) {
                    self->field_022c = 0;
                    result = CallAcquireHelper712F60(secondDevice);
                }
            }
        } else {
            uint32_t firstDevice = self->field_0020;
            if (firstDevice) {
                CallDeviceSlot8(firstDevice);
            }

            uint32_t secondDevice = self->field_0024;
            result = static_cast<char>(secondDevice);
            if (secondDevice && self->field_0c91) {
                result = CallDeviceSlot8(secondDevice);
            }
        }
    }

    return result;
}

HOOK(0x00713030, CInputDirectInput_SetAcquireEnabled, "auto");
