#include <cstdint>
#include <cstddef>
#include "types/auto_classes.h"
#include "hook.h"

// Sets the ROWFILL flag on the list box and marks the layout as dirty so it is
// recomputed on the next layout pass. Returns the value that was stored.
static char __fastcall CGuiListBoxBase_SetRowFill(CGuiListBoxBase* self, void* /*edx*/, char value)
{
    self->m_bLayoutDirty = 1;
    self->m_bRowFill = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x007162F0, CGuiListBoxBase_SetRowFill, "auto");
