#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the literal tag name of the "stunned" (aturdido) animation/state.
static const char* __cdecl GetStunnedTag()
{
    return reinterpret_cast<const char*>(0x837254); // "TFncAturdido"
}

HOOK(0x4AA090, GetStunnedTag, "auto");
