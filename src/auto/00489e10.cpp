#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));

static int __fastcall C_00833708_FindValueIndex(C_00833708* self, void* /*edx*/, int value)
{
    if (self->field_0014 == 0)
        return -1;

    uint32_t index = 0;
    uint32_t offset = 0;
    do
    {
        void* handle = CSF_AT(void*, reinterpret_cast<void*>(self->field_000c), offset);
        int valuePointer = Handle_GetValuePointer(handle, nullptr);
        if (CSF_AT(int, reinterpret_cast<void*>(valuePointer), 0) == value)
            return static_cast<int>(index);

        ++index;
        offset += 96;
    } while (index < self->field_0014);

    return -1;
}

HOOK(0x00489E10, C_00833708_FindValueIndex, "auto");
