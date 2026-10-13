#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static uint32_t __cdecl InputDInput_SetBufferSize(hook::Regs* regs, const uint32_t* args)
{
    void* device = reinterpret_cast<void*>(regs->eax);

    alignas(8) uint8_t propertyBytes[0x14];
    uint32_t* property = reinterpret_cast<uint32_t*>(propertyBytes);
    property[2] = 0;
    property[3] = 0;
    property[4] = args[0];
    property[0] = 20;
    property[1] = 16;

    return static_cast<uint32_t>(
        ((int (__stdcall*)(void*, uint32_t, uint32_t*))
            CSF_AT(void**, device, 0)[0x18 / 4])(device, 1, property));
}

HOOK_REGS(0x712F80, InputDInput_SetBufferSize, 0, "input_dinput");
