#include "globals.h"
#include "hook.h"

// Returns the name of the "buscar campo abierto" process (TPrcBuscarCampoAbierto).
static const char* __cdecl TPrcBuscarCampoAbierto_GetName()
{
    return CSF_STR(0x839CB8, "TPrcBuscarCampoAbierto");
}

HOOK(0x4AE4F0, TPrcBuscarCampoAbierto_GetName, "auto");
