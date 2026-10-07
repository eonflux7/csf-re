#include <cstdint>
#include "globals.h"
#include "hook.h"

// Global read by this stub; purpose unknown (single 4-byte value).
CSF_GLOBAL(int32_t, g_Value_8bbf2c, 0x008BBF2C);

// 0x0070DDE0: returns the current value of the global at 0x008BBF2C.
static int __cdecl GetGlobalValue()
{
  return g_Value_8bbf2c;
}

HOOK(0x0070DDE0, GetGlobalValue, "auto");
