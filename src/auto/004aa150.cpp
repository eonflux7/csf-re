#include "hook.h"
#include "globals.h"

// Returns a pointer to the constant string "TProtegerse" located at 0x8379EC.
// The original simply does `mov eax, 0x8379ec ; ret`, so the exact address is
// returned to preserve pointer identity with the game's literal.
static const char* __cdecl GetTProtegerseString()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x8379EC));
}

HOOK(0x004AA150, GetTProtegerseString, "auto");
