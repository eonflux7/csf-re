#include <cstdint>
#include "hook.h"

// Returns the pointer to the constant Spanish label string "TSentarse"
// (stored in the game's read-only data at 0x834F80).
static const char* __cdecl GetSentarseLabel()
{
    return reinterpret_cast<const char*>(0x834F80);
}

HOOK(0x4A7DB0, GetSentarseLabel, "auto");
