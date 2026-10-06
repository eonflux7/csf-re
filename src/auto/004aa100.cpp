#include <cstdint>
#include "hook.h"

// Returns the name of the "TExplosivos" type/class entry (global string literal
// owned by the game at 0x008367FC). Uses the game's own buffer so pointer
// comparisons against this name keep working.
static const char* __cdecl GetTExplosivosName()
{
    return reinterpret_cast<const char*>(0x008367FC);
}

HOOK(0x004AA100, GetTExplosivosName, "auto");
