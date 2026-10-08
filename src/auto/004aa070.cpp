#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Returns the name of the "exit interactive mode" task (Spanish: "TSalirDeInteractivo").
// Original: mov eax, 0x8365CC ; ret   (0x004AA070, 6 bytes)
static const char* __cdecl TSalirDeInteractivo()
{
    return reinterpret_cast<const char*>(CSF_ADDR_DATA(0x008365CC));
}

HOOK(0x4AA070, TSalirDeInteractivo, "auto");
