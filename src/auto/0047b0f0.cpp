#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the literal string "TModoAutomatico" (stored at 0x832220).
static __cdecl const char* GetModoAutomaticoName()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x832220));
}

HOOK(0x47B0F0, GetModoAutomaticoName, "auto");
