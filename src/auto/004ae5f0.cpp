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

// 0x004AE5F0 - returns the constant name of the "Vigilar desde parapeto" command.
// Disassembly: mov eax, 0x839E8C ; ret  (no stack args, ecx/edx unused)
static const char* __fastcall C_00839e08_GetVigilarDesdeParapetoName(C_00839e08* /*self*/, void* /*edx*/)
{
    return CSF_STR(0x00839E8C, "TFncVigilarDesdeParapeto");
}

HOOK(0x004AE5F0, C_00839e08_GetVigilarDesdeParapetoName, "auto");
