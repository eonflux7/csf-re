#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global pointer stored at 0x008A6788; the original getter simply returns it.
CSF_GLOBAL(void*, g_off_008A6788, 0x008A6788);

// _UNKNOWN **sub_69D5B0()
//   mov eax, dword ptr [0x8a6788]
//   ret
static void** __cdecl Get_GlobalObject_008A6788()
{
    return static_cast<void**>(g_off_008A6788);
}

HOOK(0x0069D5B0, Get_GlobalObject_008A6788, "auto");
