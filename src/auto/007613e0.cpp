#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, statusTexts, 0x008A969C);
static const char* __cdecl QueryLinkedStatusText(int index) {
    return (&statusTexts)[index];
}
HOOK(0x007613E0, QueryLinkedStatusText, "auto");
