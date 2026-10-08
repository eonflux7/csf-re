#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the literal identifier "TSubido" (address 0x834B00 in the game image).
static const char* __cdecl GetTSubido()
{
  return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x834B00));
}

HOOK(0x4A6EB0, GetTSubido, "auto");
