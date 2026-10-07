#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of the sub-object embedded at offset 0xD8 of this instance.
static char* __fastcall CGuiControl_GetSubObject0D8(CGuiControl* self, void* /*edx*/)
{
    return reinterpret_cast<char*>(self) + 0xd8;
}

HOOK(0x00718030, CGuiControl_GetSubObject0D8, "auto");
