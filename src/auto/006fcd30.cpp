#include <cstdint>
#include <cstddef>
#include "types/auto_classes.h"
#include "hook.h"

// 0x006FCD30: stores the given value into field_0030 and returns it.
static int __fastcall CLightObject_SetField0030(CLightObject* self, void* /*edx*/, int value)
{
    self->field_0030 = static_cast<uint32_t>(value);
    return value;
}

HOOK(0x6FCD30, CLightObject_SetField0030, "auto");
