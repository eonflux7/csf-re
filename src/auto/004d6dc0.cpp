#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the pointer to the constant string "TIAOff" embedded in the game image.
static const char* __cdecl GetTIAOffString()
{
  return CSF_STR(0x83C828, "TIAOff");
}

HOOK(0x4D6DC0, GetTIAOffString, "auto");
