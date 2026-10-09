#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the object itself (identity / "get this" accessor).
static void* __fastcall C_008310b8_GetSelf(C_008310b8* self, void* /*edx*/)
{
    return self;
}

// LTCG callers (0x747EB0) keep a value in ecx across the call: the original never writes ecx.
HOOK_KEEP_ECX(0x4B2980, C_008310b8_GetSelf, 0, "auto");
