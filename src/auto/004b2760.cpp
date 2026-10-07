#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the static type-name string of the "TPrcBuscarInteractivo" task process.
// Original: mov eax, 0x83A5A8 ; ret  (__cdecl, no arguments, returns const char*)
static const char* __cdecl GetBuscarInteractivoTypeName()
{
    return CSF_STR(0x83A5A8, "TPrcBuscarInteractivo");
}

HOOK(0x4B2760, GetBuscarInteractivoTypeName, "auto");
