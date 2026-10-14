#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00863044_FindFirstIndexByKey, 0x006FE4F0, (C_00863044* self, void* , int key));

static int __fastcall C_00863044_GetEntryValueByKey(
    C_00863044* self, void* /*edx*/, int key)
{
    const int index = C_00863044_FindFirstIndexByKey(self, nullptr, key);
    if (index == -1)
        return 0;

    // The original address arithmetic wraps at 32 bits.
    const uint32_t valueAddress =
        self->field_0024 + 12 * static_cast<uint32_t>(index) + 8;
    return CSF_AT(int, reinterpret_cast<void*>(valueAddress), 0);
}

HOOK(0x006FE8C0, C_00863044_GetEntryValueByKey, "auto");
