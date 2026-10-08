#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the internal class/function name string for this object: "TFncExit".
// The original simply loads the address of the embedded string literal.
static const char* __cdecl TFncExit_GetName()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x0082F498)); // "TFncExit"
}

HOOK(0x00420760, TFncExit_GetName, "auto");
