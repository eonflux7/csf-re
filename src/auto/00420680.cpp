#include <cstdint>
#include "hook.h"

// Returns the literal string "TAturdido" (stunned/dazed state name).
static const char* __cdecl GetTAturdidoString()
{
    return "TAturdido";
}

HOOK(0x420680, GetTAturdidoString, "auto");
