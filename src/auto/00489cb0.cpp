#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_00833708_GetItemValue(
    C_00833708* self, void* /*edx*/, int itemId)
{
    const int itemIndex =
        ((int(__fastcall*)(C_00833708*, void*, int))self->vtbl[48])(
            self, nullptr, itemId);

    if (itemIndex == -1)
        return itemIndex;

    void* item = CSF_AT(
        void*,
        reinterpret_cast<void*>(self->field_000c),
        static_cast<uint32_t>(itemIndex) * 96u);

    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, item, 0)[24])(
        item, nullptr, 0);
}

HOOK(0x00489CB0, C_00833708_GetItemValue, "auto");
