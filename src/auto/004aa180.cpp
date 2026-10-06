#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the animation/state name string constant "TAsomarse".
static const char* __cdecl GetTAsomarseString()
{
    return CSF_STR(0x837B94, "TAsomarse");
}

HOOK(0x4AA180, GetTAsomarseString, "auto");
