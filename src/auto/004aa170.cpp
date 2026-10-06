#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the pointer to the constant string "TEsperar" (0x837B04).
static const char* __cdecl GetEsperarString()
{
  return "TEsperar";
}

HOOK(0x4AA170, GetEsperarString, "auto");
