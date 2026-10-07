#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the localized tag string "TLevantandose" (Spanish: "Standing up").
// Original: mov eax, 0x83ccc0 ; ret
static const char* __cdecl GetLevantandoseTag()
{
    return CSF_STR(0x83CCC0, "TLevantandose");
}

HOOK(0x4D6E20, GetLevantandoseTag, "auto");
