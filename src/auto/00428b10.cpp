#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Returns a pointer to the constant string "truco".
static const char* __cdecl GetTrucoString()
{
    return "truco";
}

HOOK(0x428B10, GetTrucoString, "auto");
