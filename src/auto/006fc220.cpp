#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x006FC220 (13 bytes) — __thiscall, ret 4
// Stores the given byte into C_00862e88::field_0054 and returns it unchanged.
static char __fastcall C_00862e88_SetField0054(C_00862e88* self, void* /*edx*/, char value)
{
    self->field_0054 = static_cast<uint8_t>(value);
    return value;
}

HOOK(0x006FC220, C_00862e88_SetField0054, "auto");
