#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the constant string "TVigilar".
static const char* __cdecl GetTVigilarString()
{
    return CSF_STR(0x0083640C, "TVigilar");
}

HOOK(0x004AA040, GetTVigilarString, "auto");
