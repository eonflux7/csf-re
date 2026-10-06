#include <cstdint>
#include "hook.h"

// Returns the address of the global data block at 0x008AFDA0.
// Original: mov eax, 0x8AFDA0 ; ret
static void* __cdecl GetGlobalBlock8AFDA0()
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(0x008AFDA0));
}

HOOK(0x46ED80, GetGlobalBlock8AFDA0, "auto");
