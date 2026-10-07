// Target: 0x004379B0 — std::string-style data() accessor.
// Returns the internal character pointer: if capacity (field_0030) < 0x10 the
// small-string buffer embedded at +0x1C is returned, otherwise the heap pointer
// stored at +0x1C.

#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the string's data pointer (inline buffer when capacity < 0x10).
static char* __fastcall CGuiControl_GetStringData(CGuiControl* self, void* /*edx*/)
{
    if (self->field_0030 < 0x10u)
        return reinterpret_cast<char*>(self) + 0x1c;   // &self->field_001c
    return reinterpret_cast<char*>(self->field_001c);
}

HOOK(0x4379B0, CGuiControl_GetStringData, "auto");
