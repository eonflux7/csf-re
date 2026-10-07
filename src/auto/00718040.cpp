#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Returns the address of the sub-object embedded at offset 0xB0 of this instance.
static CGuiControl* __fastcall CGuiControl_GetSize(CGuiControl* self, void* /*edx*/)
{
    return reinterpret_cast<CGuiControl*>(reinterpret_cast<char*>(self) + 0xb0);
}

HOOK(0x718040, CGuiControl_GetSize, "auto");
