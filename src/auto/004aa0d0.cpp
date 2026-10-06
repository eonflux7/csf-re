#include <cstdint>
#include "hook.h"

// Returns the static string literal "TRastrear" (Spanish: "to track / trace").
// Original: mov eax, 0x83603C ; ret   (address of the string literal in .rdata)
static const char* __cdecl GetTRastrearString()
{
    return "TRastrear";
}

HOOK(0x4AA0D0, GetTRastrearString, "auto");
