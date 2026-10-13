#include <cstdint>
#include "globals.h"
#include "hook.h"

// Variadic __cdecl logger at 0x653BD0; bind signature copied from the packet.
CSF_FUNC(int, __cdecl, MemDebug_LogFormatted, 0x00653BD0, (int a1, int a2, ...));

// Advances a one-byte debug state machine in place and returns the state value
// it read. Unknown states are logged with "Value: %d" and the logger's result is returned.
static int __fastcall MemDebug_AdvanceStateByte(uint8_t* self, void* /*edx*/) {
    const uint8_t state = *self;
    int result = state;

    switch (state) {
    case 0u:
        return result;
    case 1u:
    case 3u:
        *self = 4;
        break;
    case 2u:
    case 5u:
        *self = 0;
        break;
    case 4u:
    case 0x84u:
        *self = 5;
        break;
    case 0x81u:
    case 0x83u:
        *self = static_cast<uint8_t>(-124);
        break;
    default:
        result = MemDebug_LogFormatted(4, reinterpret_cast<int>(CSF_STR(0x0085E310, "Value: %d")), state);
        break;
    }
    return result;
}

HOOK(0x00690B20, MemDebug_AdvanceStateByte, "auto");
