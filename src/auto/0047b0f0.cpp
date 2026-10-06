#include <cstdint>
#include "hook.h"

// Returns the literal string "TModoAutomatico" (stored at 0x832220).
static __cdecl const char* GetModoAutomaticoName()
{
    return reinterpret_cast<const char*>(0x832220);
}

HOOK(0x47B0F0, GetModoAutomaticoName, "auto");
