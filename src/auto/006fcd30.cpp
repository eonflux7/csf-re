#include <cstdint>
#include <cstddef>
#include "types/auto_classes.h"
#include "hook.h"

// 0x006FCD30: stores the given value into field_0030 and returns it.
static int __fastcall C_00862f70_SetField0030(C_00862f70* self, void* /*edx*/, int value)
{
    self->field_0030 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x6FCD30, C_00862f70_SetField0030, "auto");
