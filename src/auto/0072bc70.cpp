#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Reads the byte flag at offset 0x158 of CGuiPanel and returns it.
static char __fastcall CGuiPanel_GetField0158(CGuiPanel* self, void* /*edx*/)
{
    return static_cast<char>(self->field_0158); // [ecx+0x158]
}

HOOK(0x0072BC70, CGuiPanel_GetField0158, "auto");
