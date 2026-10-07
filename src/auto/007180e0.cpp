#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the byte at offset 0xCA (inside C_00830438::pad_00a8).
static char __fastcall CGuiControl_IsMouseEnabled(CGuiControl* self, void* /*edx*/)
{
    return static_cast<char>(*reinterpret_cast<uint8_t*>(reinterpret_cast<char*>(self) + 0xCA));
}

HOOK(0x7180E0, CGuiControl_IsMouseEnabled, "auto");
