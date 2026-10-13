#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(int, g_NetTransportObject, 0x008BC13C);

// Resets the cached net transport object held in g_NetTransportObject: if set, calls its
// vtable slot 0 with flag 1 (destroy-with-free), then clears the global.
static int __cdecl NetSession_ResetTransportState()
{
    int result = 0;
    if (g_NetTransportObject != 0)
    {
        void* transport = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(g_NetTransportObject)));
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, transport, 0)[0])(transport, nullptr, 1);
        g_NetTransportObject = 0;
    }
    return result;
}

HOOK(0x007437C0, NetSession_ResetTransportState, "auto");
