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

// Forwards a display mode flag (0 or 1) to the child object stored at +0x14.
// The child's vtable slot at +0xA8 is invoked with the chosen mode; when the
// child pointer is null, the function returns 0 without calling anything.
static int __fastcall CMenuMpScoreBar_ApplyChildDisplayMode(CMenuMpScoreBar* self, void* /*edx*/)
{
    uint32_t childRaw = self->field_0014;
    if (childRaw == 0)
    {
        return 0;
    }

    void* child = reinterpret_cast<void*>(childRaw);

    // Mode 0 when both flags are set, otherwise mode 1.
    int mode = 1;
    if (self->field_0028 != 0 && self->field_0038 != 0)
    {
        mode = 0;
    }

    // Vtable slot at byte offset 0xA8 (index 42).
    void** childVtable = CSF_AT(void**, child, 0);
    return ((int(__fastcall*)(void*, void*, int))(childVtable[0xA8 / 4]))(child, nullptr, mode);
}

HOOK(0x5408F0, CMenuMpScoreBar_ApplyChildDisplayMode, "auto");
