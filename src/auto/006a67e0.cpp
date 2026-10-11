#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original data (bound by address).
CSF_GLOBAL(uint32_t, g_LogStateWordA, 0x008A6C20);
CSF_GLOBAL(uint32_t, g_LogStateWordB, 0x008A6C24);

// Writes the log window's two state words and the addresses of two data
// blocks into the four caller-supplied output slots. Always returns 0.
static int __cdecl LogWindow_GetStateAddresses(uint32_t* outStateA,
                                               uint32_t* outStateB,
                                               uint32_t* outBlockA,
                                               uint32_t* outBlockB) {
    *outStateA = g_LogStateWordA;
    *outStateB = g_LogStateWordB;
    *outBlockA = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008A6C28)));
    *outBlockB = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008A7C28)));
    return 0;
}

HOOK(0x006A67E0, LogWindow_GetStateAddresses, "auto");
