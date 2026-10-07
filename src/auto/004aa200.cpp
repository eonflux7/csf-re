#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// 0x004AA200 (6 bytes) - returns the address of the constant string "TLevantarse".
static const char* __cdecl GetLevantarseTokenName()
{
    return CSF_STR(0x834BC0, "TLevantarse");
}

HOOK(0x4AA200, GetLevantarseTokenName, "auto");
