#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00726BF0: __stdcall, one stack arg (ret 4).
// If the passed object pointer is non-null, increments the dword at +0x54,
// then returns the original argument in eax.
static int __stdcall IncrementField54AndReturn(C_008655e8* object)
{
    if (object != nullptr)
        ++object->field_0054;
    return reinterpret_cast<int>(object);
}

HOOK(0x00726BF0, IncrementField54AndReturn, "auto");
