#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns whether field_01e0 of the object equals 3 (some state/kind tag).
static int __fastcall CMenuMpSessions_IsField01E0Equal3(CMenuMpSessions* self, void* /*edx*/)
{
    return self->field_01e0 == 3;
}

HOOK(0x00561970, CMenuMpSessions_IsField01E0Equal3, "auto");
