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
static const char* __fastcall TFunctionRecargar_GetTFunctionRecargar(TFunctionRecargar* self, void* /*edx*/)
{
    (void)self;
    return CSF_STR(0x00832ED8, "TFunctionRecargar");
}

HOOK(0x0047C320, TFunctionRecargar_GetTFunctionRecargar, "auto");
