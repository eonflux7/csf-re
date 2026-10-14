#include <cstdint>
#include "globals.h"
#include "hook.h"

static uint32_t __cdecl InputDinput_AcquireDeviceRetryWhileInputLost(
    hook::Regs* regs, const uint32_t* /*args*/)
{
    void* device = reinterpret_cast<void*>(regs->esi);

    uint32_t result =
        (*reinterpret_cast<uint32_t(__stdcall***)(void*)>(device))
            [0x1C / 4](device);

    while (result == 0x8007001E) {
        result =
            (*reinterpret_cast<uint32_t(__stdcall***)(void*)>(device))
                [0x1C / 4](device);
    }

    return result;
}

HOOK_REGS(0x712F60, InputDinput_AcquireDeviceRetryWhileInputLost, 0, "input_dinput");
