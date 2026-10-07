#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// COptions* g_pOptions : global options object pointer (0x008BAF8C)
CSF_GLOBAL(void*, g_pOptions, 0x008BAF8C);

// 0x00697390 (17 bytes, cdecl, ret 0)
// Stores the given byte into the options object's field at offset 0x41 and
// returns that same value.
static char __cdecl SetOptionsByte41(char value)
{
  CSF_AT(uint8_t, g_pOptions, 0x41) = static_cast<uint8_t>(value);
  return value;
}

HOOK(0x00697390, SetOptionsByte41, "auto");
