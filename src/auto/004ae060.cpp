#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x004AE060: returns a pointer to the constant string "TEstatico" stored at 0x00839084.
static const char* __cdecl GetStaticName()
{
    return CSF_STR(0x00839084, "TEstatico");
}

HOOK(0x4AE060, GetStaticName, "auto");
