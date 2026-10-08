#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the address of the static string "TIrASalida" (data at 0x834C80).
static const char* __cdecl GetTIrASalidaString()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x834C80));
}

HOOK(0x4A6EE0, GetTIrASalidaString, "auto");
