#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x0072FF80: trivial accessor returning the dword at offset 0x134 of the object.
static int __fastcall CGuiStatic_GetField0134(CGuiStatic* self, void* /*edx*/)
{
    return static_cast<int>(self->field_0134);
}

HOOK(0x72FF80, CGuiStatic_GetField0134, "auto");
