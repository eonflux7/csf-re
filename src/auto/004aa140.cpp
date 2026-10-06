#include <cstdint>
#include "hook.h"

// Returns a pointer to the constant string "TIrHacia" (stored in .rdata at 0x8359C4).
static const char* __cdecl GetTIrHaciaString()
{
    return "TIrHacia";
}

HOOK(0x4AA140, GetTIrHaciaString, "auto");
