#include <cstdint>
#include "hook.h"

// Maps the session-mode byte at offset +2 of the argument to a mode id.
// The original sign-extends the byte (movsx), subtracts 0x44 and range-checks 0..0x10,
// so only 'D' (68), 'P' (80), 'S' (83) and 'T' (84) produce non-zero results and
// every other value returns 0.
static int __cdecl MenuMpSessions_GetSessionMode(int a1)
{
    const int8_t* packet = reinterpret_cast<const int8_t*>(static_cast<uintptr_t>(a1));
    int8_t sessionModeCode = packet[2];
    int result = 0;
    switch (static_cast<int>(sessionModeCode))
    {
    case 68:   // 'D'
        result = 0;
        break;
    case 80:   // 'P'
        result = 2;
        break;
    case 83:   // 'S'
        result = 3;
        break;
    case 84:   // 'T'
        result = 1;
        break;
    default:
        return result;
    }
    return result;
}

HOOK(0x749770, MenuMpSessions_GetSessionMode, "auto");
