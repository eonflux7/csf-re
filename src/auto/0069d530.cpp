#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, _atexit, 0x007AEE43, (void (__cdecl* callback)()));

CSF_GLOBAL(uint32_t, g_ErrorLogInitFlags, 0x008BB3E8);
CSF_GLOBAL(void*, g_ErrorLogStream, 0x008BB3E0);
CSF_GLOBAL(char, g_ErrorLogBuffer, 0x008BB3E4);

// Lazily initializes the CErrorLog buffer on first use and returns its address.
static char* __cdecl CErrorLog_GetLogBuffer()
{
    if ((g_ErrorLogInitFlags & 1) == 0)
    {
        g_ErrorLogInitFlags |= 1u;
        g_ErrorLogBuffer = 0;
        g_ErrorLogStream = nullptr;
        _atexit(reinterpret_cast<void (__cdecl*)()>(CSF_ADDR_FN(0x0082ADF0)));
    }
    return &g_ErrorLogBuffer;
}

HOOK(0x0069D530, CErrorLog_GetLogBuffer, "auto");
