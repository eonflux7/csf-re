#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the tag string identifying impact effects. The original stores the
// pointer to the literal 'TImpacto' and returns it unchanged.
static const char* __cdecl GetImpactoTag()
{
    return CSF_STR(0x82EA18, "TImpacto");
}

HOOK(0x4206A0, GetImpactoTag, "auto");
