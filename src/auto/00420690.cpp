#include <cstdint>
#include "hook.h"

// Returns the pointer to the static string "TGirando" (located at 0x82EB98).
static const char* __cdecl GetTGirandoLabel()
{
    return reinterpret_cast<const char*>(0x82EB98);
}

HOOK(0x420690, GetTGirandoLabel, "auto");
