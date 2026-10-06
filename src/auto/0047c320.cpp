#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the literal name of the "Recargar" (reload) type-function.
// 0047C320: mov eax, 0x832ED8 ; ret
static const char* __fastcall C_00832e40_GetTFunctionRecargar(C_00832e40* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x00832ED8, "TFunctionRecargar");
}

HOOK(0x0047C320, C_00832e40_GetTFunctionRecargar, "auto");
