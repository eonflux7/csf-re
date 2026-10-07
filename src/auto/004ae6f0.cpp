#include <cstdint>
#include "globals.h"
#include "hook.h"

// Returns the constant name string "TFncLanzarGranada".
// Original: mov eax, 0x83A204 ; ret  (__cdecl, no args)
static const char* __cdecl GetTFncLanzarGranadaName()
{
    return CSF_STR(0x83A204, "TFncLanzarGranada");
}

HOOK(0x4AE6F0, GetTFncLanzarGranadaName, "auto");
