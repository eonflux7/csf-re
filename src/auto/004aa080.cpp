#include <cstdint>
#include "hook.h"

// Returns a pointer to the (read-only) string "TRendirse".
// Original: mov eax, 0x83713c ; ret   (i.e. a constant string address)
static const char* __cdecl GetTRendirse()
{
    return "TRendirse";
}

HOOK(0x004AA080, GetTRendirse, "auto");
