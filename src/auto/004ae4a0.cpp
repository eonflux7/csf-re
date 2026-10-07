#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Returns the static name string of the "buscar enemigo" process/state.
static const char* __cdecl TPrcBuscarEnemigo_GetName()
{
    return CSF_STR(0x00839A38, "TPrcBuscarEnemigo");
}

HOOK(0x004AE4A0, TPrcBuscarEnemigo_GetName, "auto");
