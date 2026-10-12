#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, sub_489F50, 0x00489F50, (void* self, void* edx, int a2));
static CSF_FUNC(int, __fastcall, sub_48A030, 0x0048A030, (void* self, void* edx, int a2));

static int __fastcall C_00833708_SetEntryState(
    C_00833708* self, void* /*edx*/, int index, int state)
{
    const uint32_t entryAddress =
        self->field_000c + 96u * static_cast<uint32_t>(index);
    CSF_AT(int, reinterpret_cast<void*>(entryAddress), 0x04) = state;

    if (state == 2)
        return sub_489F50(self, nullptr, index);
    if (state == 3)
        return sub_48A030(self, nullptr, index);
    return index;
}

HOOK(0x0048A270, C_00833708_SetEntryState, "auto");
